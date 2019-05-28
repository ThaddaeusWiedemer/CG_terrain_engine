#version 410 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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

vec3 calculateSpecular(Material material, vec3 shaderPosition, vec3 shaderNormal, vec3 lightPos0, vec3 cameraPos){
	vec3 lightToPosDirVec = normalize(shaderPosition - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(shaderNormal)));
	vec3 posToViewDirVec = normalize(cameraPos - shaderPosition);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
	vec3 specularFinal = material.specular * specularConstant * texture(material.specularTex, shaderTexCoord).rgb;

	return specularFinal;
}

void main(){
	//Ambient light
	vec3 ambientFinal = calculateAmbient(material);

	//Diffuse light
	vec3 diffuseFinal = calculateDiffuse(material, shaderPosition, shaderNormal, lightPos0);

	//Specular light
	vec3 specularFinal = calculateSpecular(material, shaderPosition, shaderNormal, lightPos0, cameraPos);

    //finalColor = texture(texture, shaderTexCoord) * shaderColor
	//Final light
	finalColor = 	texture(material.diffuseTex, shaderTexCoord) * shaderColor
		* (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}