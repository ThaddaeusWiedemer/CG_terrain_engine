#version 410 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in vec2 vertex_texCoord;
layout(location = 3) in vec3 vertex_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float tiling;

out vec3 worldSpace;
out vec4 clipSpace;
out vec4 color;
out vec2 texCoords;
out vec3 normal;

void main(){
    worldSpace = vec4(model * vec4(vertex_position, 1.f)).xyz;
	clipSpace = projection * view * model * vec4(vertex_position, 1.f);

	color = vertex_color;
	texCoords = vec2(vertex_texCoord.x, 1.f - vertex_texCoord.y) * tiling;
	normal = normalize(mat3(model) * vertex_normal);
	
	gl_Position = clipSpace;
}