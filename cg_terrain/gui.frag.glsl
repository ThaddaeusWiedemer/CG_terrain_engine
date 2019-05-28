#version 410 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
	sampler2D displaceTex;
};

in vec4 color;
in vec2 texCoords;
flat in int renderDepth;

uniform Material material;

out vec4 finalColor;

void main(){
	if(renderDepth == 1){
		float near = .1f;
		float far  = 1000.f;
		float depth = texture(material.diffuseTex, texCoords).r;
		depth = (2.0 * near) / (far + near - depth * (far - near));
		finalColor = vec4(depth, depth, depth, 1.f);
	}else{
		finalColor = texture(material.diffuseTex, texCoords);
	}
	//finalColor = color;
}