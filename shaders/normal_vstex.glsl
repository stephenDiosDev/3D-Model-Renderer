#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out Data
{
    vec3 position;
    vec3 normal;
} data;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//move vertices in model space to world space
    data.position = vec3(model * vec4(position, 1.0));
    data.normal = normalize(mat3(model) * normal);
	
    gl_Position = projection * view * vec4(data.position, 1.0);
}