#version 330 core

layout(location = 0) in vec2 vertexPosition_screenspace;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

uniform int width = 1280;
uniform int height = 720;

void main(){

	// Output position of the vertex, in clip space
	// map [0..1280][0..720] to [-1..1][-1..1]
	vec2 vertexPosition_homoneneousspace = vertexPosition_screenspace - vec2(width / 2, height / 2);
	vertexPosition_homoneneousspace /= vec2(width / 2, height / 2);
	gl_Position =  vec4(vertexPosition_homoneneousspace,0,1);

	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}
