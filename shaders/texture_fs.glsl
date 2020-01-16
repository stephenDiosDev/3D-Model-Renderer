#version 330 core
out vec4 fragColor;

in Data {
	vec2 texCoord;
} data;

uniform sampler2D tex;

void main()
{
        vec3 c = texture(tex, data.texCoord).rgb;
    fragColor = vec4(c, 1.0);
}
