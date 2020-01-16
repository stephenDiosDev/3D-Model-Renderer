#version 330 core
out vec4 fragColor;

in Data {
        vec3 colour;
        vec2 texCoord;
} data;
uniform sampler2D tex;

void main()
{
    vec3 texColour = texture(tex, data.texCoord).rgb;
    fragColor = vec4(data.colour * texColour, 1.0);
}
