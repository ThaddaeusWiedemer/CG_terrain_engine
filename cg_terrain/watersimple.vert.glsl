#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 texCoordScale;
uniform vec2 texCoordOffset;

out vec3 shaderPosition;
out vec4 shaderColor;
out vec2 shaderTexCoord;
out vec3 shaderNormal;

void main(){
    shaderPosition = vec4(model * vec4(position, 1.f)).xyz;
	shaderColor = color;
	shaderTexCoord = (vec2(texCoord.x, 1.f - texCoord.y) + texCoordOffset) * texCoordScale;
	shaderNormal = normalize(mat3(model) * normal);

	gl_Position = projection * view * model * vec4(position, 1.f);
}