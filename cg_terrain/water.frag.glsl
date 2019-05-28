#version 410 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
	sampler2D displaceTex;
};

in vec3 worldSpace;
in vec4 clipSpace;
in vec4 color;
in vec2 texCoords;
in vec3 normal;

uniform Material material;
uniform vec3 lightPos0;
uniform vec3 cameraPos;
uniform float moveFactor;
uniform sampler2D depthMap;
uniform int doFresnel;
uniform int doEdge;

const float waveStrength = .3f;
const float shineDamper = 35.f;
const vec3 highlightColor = vec3(1.f);
const vec4 waterColor = vec4(0.f, .2f, .5f, 1.f);

out vec4 finalColor;	

void main(){
	// compute coordinates for reflection texture
	vec2 normalizedDeviceSpace = (clipSpace.xy / clipSpace.w) / 2.f + .5f;
	vec2 reflectionTexCoords = vec2(normalizedDeviceSpace.x, - normalizedDeviceSpace.y);

	// compute water depth
	float near = .1f;
	float far  = 2.f;
	float depth = texture(depthMap, normalizedDeviceSpace).r;
	float floorDistance = (2.0 * near) / (far + near - depth * (far - near));
	float waterDistance = (2.0 * near) / (far + near - gl_FragCoord.z * (far - near));
	float waterDepth = floorDistance - waterDistance;
	float edgeEffect = clamp(waterDepth/.01f, 0.f, .1f);
	if(doEdge == 0){
		edgeEffect = .1f;
	}

	// sample displacement map and use on reflection coordinates
	// variant 2 for displacement, looks better on large bodies of water
	vec2 displacement1 = (texture(material.displaceTex, vec2(texCoords.x + moveFactor, texCoords.y)).rg * 2.f - 1.f) * waveStrength;
	vec2 displacement2 = (texture(material.displaceTex, vec2(-texCoords.x  + moveFactor, texCoords.y + moveFactor)).rg * 2.f - 1.f) * waveStrength;
	vec2 totalDisplacement = (displacement1 + displacement2) * edgeEffect;

	// variant 2 for displacement, looks better on smaller bodies of water
	//vec2 displacement = (texture(material.displaceTex, vec2(texCoords.x + moveFactor, texCoords.y)).rg * 2.f - 1.f) * waveStrength;
	//vec2 displacedTexCoords = texCoords + vec2(displacement.x, displacement.y + moveFactor);
	//vec2 totalDisplacement = (texture(material.displaceTex, displacedTexCoords).rg * 2.f - 1.f) * waveStrength;
	
	reflectionTexCoords += totalDisplacement;
	reflectionTexCoords.x = clamp(reflectionTexCoords.x, .001f, .999f);
	reflectionTexCoords.y = clamp(reflectionTexCoords.y, -.999f, -.001f);
	vec4 reflectColor = texture(material.diffuseTex, reflectionTexCoords);

	// sample normal map for specular highlights (specular texture used as normal map)
	vec4 normalMapColor1 = texture(material.specularTex, vec2(texCoords.x + moveFactor, texCoords.y)) * waveStrength;
	vec4 normalMapColor2 = texture(material.specularTex, vec2(-texCoords.x + moveFactor, texCoords.y + moveFactor)) * waveStrength;
	vec4 totalNormalMapColor = normalize(normalMapColor1 + normalMapColor2);
	vec3 normal = normalize(vec3(totalNormalMapColor.r * 2.f - 1.f, totalNormalMapColor.b * .1f + 2.f, totalNormalMapColor.g * 2.f - 1.f));
	
	// calculate specular highlights (for white light)
	vec3 fromLightVector = normalize(worldSpace - lightPos0);
	vec3 reflectedLight = normalize(reflect(fromLightVector, normal));
	vec3 toViewVector = normalize(cameraPos - worldSpace);
	float specularConstant = pow(max(dot(toViewVector, reflectedLight), 0), shineDamper);
	vec4 specularHighlights = vec4(material.specular * specularConstant * highlightColor, 1.f);

	// include Fresnel effect (assuming waterColor as refractive Color and horizontal water surface)
	float refractiveFactor = dot(toViewVector, vec3(.0f, 1.f, 0.f));
	if(doFresnel == 0){
		refractiveFactor = .3f;
	}

	finalColor = mix(reflectColor, waterColor, refractiveFactor) + specularHighlights;
}