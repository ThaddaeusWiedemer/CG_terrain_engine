#version 410 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec2 shaderTexCoord;

uniform Material material;

out vec4 finalColor;

void main(){
    finalColor = texture(material.diffuseTex, shaderTexCoord);
}