#version 330 core

// Uniforms
uniform mat4 camera;
uniform mat4 model;

// Input data
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

// Output data to fragment shader
out vec3 fragmentColor;
out vec3 fragmentNormal;
out vec3 fragmentPosition;

void main(){

	// Position of vertex multiplied by projection
	gl_Position =  camera * model * vec4(position,1);

	// Send color and normal to fragment shader
	fragmentColor = color;
	fragmentNormal = normal;
	fragmentPosition = position;
}
