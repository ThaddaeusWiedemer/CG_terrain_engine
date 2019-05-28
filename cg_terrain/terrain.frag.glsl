#version 410 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec3 shaderPosition;
in vec4 shaderColor;
in vec2 shaderTexCoord;
in vec3 shaderNormal;

uniform Material material;
uniform vec3 lightPos0;
uniform vec3 cameraPos;
uniform vec2 detailTexScale;

out vec4 finalColor;

vec3 calculateAmbient(Material material){
	return material.ambient;
}

vec3 calculateDiffuse(Material material, vec3 shaderPosition, vec3 shaderNormal, vec3 lightPos0){
	vec3 posToLightDirVec = normalize(lightPos0 - shaderPosition);
	float diffuse = clamp(dot(posToLightDirVec, shaderNormal), 0, 1);
	vec3 diffuseFinal = material.diffuse * diffuse;

	return diffuseFinal;
}

void main(){
	//Ambient light
	vec3 ambientFinal = calculateAmbient(material);

	//Diffuse light
	vec3 diffuseFinal = calculateDiffuse(material, shaderPosition, shaderNormal, lightPos0);

	// final color. use the specular texture as a detail texture here
	finalColor = (texture(material.diffuseTex, shaderTexCoord) + texture(material.specularTex, shaderTexCoord * detailTexScale) - vec4(.5f, .5f, .5f, 0.f))
		* (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f));
}