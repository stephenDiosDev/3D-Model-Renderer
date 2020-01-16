#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out Data
{
    vec3 colour;
} data;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;
uniform vec3 lightColour = vec3(1.0, 1.0, 1.0);

void main()
{
        //move vertices in model space to world space
    vec3 pos = vec3(model * vec4(position, 1.0));
    vec3 norm = normalize(mat3(model) * normal);
    gl_Position = projection * view * vec4(pos, 1.0);

    //gouraud uses ambient or only the highlight can be seen
    float ambientLight = 0.2;
    vec3 ambient = ambientLight * lightColour;

    //find the light direction

    vec3 lightDirection = normalize(pos - lightPosition);
    float diff = max(dot(norm, lightDirection), 0.0); //dot prod
    vec3 diffuse = lightColour * diff * 0.3;

    vec3 result = (ambient + diffuse);

    data.colour = result;
}
