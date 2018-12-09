#version 330

layout(location = 0) in  vec3 in_Position;	//Every time this will be a different vertex position
uniform mat4 MVP;

void main () {
	gl_Position = MVP * vec4 (in_Position, 1.0);
};