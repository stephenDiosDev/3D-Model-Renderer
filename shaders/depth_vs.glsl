#version 330 core
layout (location = 0) in vec3 position;

out Data
{
    vec3 position;
} data;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//move vertices in model space to world space
    data.position = vec3(model * vec4(position, 1.0));
	
    gl_Position = projection * view * vec4(data.position, 1.0);
}