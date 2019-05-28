#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 shaderColor;
out vec2 shaderTexCoord;

void main(){
	shaderColor = color;
	shaderTexCoord = texCoord;
	
	gl_Position = projection * view * model * vec4(position, 1.f);
}