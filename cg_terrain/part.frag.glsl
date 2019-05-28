#version 410 core

in vec4 shaderColor;
in vec2 shaderTexCoord;

uniform sampler2D sprite;

out vec4 finalColor;

void main(){
	finalColor = texture(sprite, shaderTexCoord) * shaderColor;
}