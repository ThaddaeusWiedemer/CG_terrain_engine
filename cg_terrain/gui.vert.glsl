#version 410 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in vec2 vertex_texCoord;
layout(location = 3) in vec3 vertex_normal;

uniform mat4 projection;
uniform vec2 offset;
uniform int flip;

out vec4 color;
out vec2 texCoords;
flat out int renderDepth;

void main(){
	renderDepth = flip;
	color = vertex_color;
	texCoords = vec2(vertex_texCoord.x, vertex_texCoord.y * flip);
	gl_Position = vec4(vertex_position.x / 2.f + offset.x, vertex_position.y / 2.f + offset.y, 0.f, 1.f);
}