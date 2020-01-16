#version 330 core
out vec4 fragColor;

in Data {
	vec3 position;
} data;

uniform vec3 lightPosition;

void main()
{
	float d = length(data.position - lightPosition);
	vec3 c;
	if(d < 1.f) {
		c = vec3(0,1,1); 
	}
	else {
		c = vec3(1,0,0);
	}
    fragColor = vec4(c, 1.0);
}