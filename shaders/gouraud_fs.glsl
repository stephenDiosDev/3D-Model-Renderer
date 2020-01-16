#version 330 core
out vec4 fragColor;

in Data {
        vec3 colour;
} data;

void main()
{
    fragColor = vec4(data.colour, 1.0);
}
