#version 410 core

layout(vertices = 25) out;

uniform float uOuterH, uOuterV, uInnerV, uInnerH;

void main(){
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	// set tessellation levels
	gl_TessLevelOuter[0] = uOuterH;
    gl_TessLevelOuter[1] = uOuterV;
    gl_TessLevelOuter[2] = uOuterH;
    gl_TessLevelOuter[3] = uOuterV;
    gl_TessLevelInner[0] = uInnerV;
    gl_TessLevelInner[1] = uInnerH;
}