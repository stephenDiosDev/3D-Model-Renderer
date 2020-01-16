#version 330 core
out vec4 fragColor;

in Data
{
    vec3 position;
    vec3 norm;
    vec2 texCoord;
} data;

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 lightColour = vec3(1.0, 1.0, 1.0);
uniform sampler2D tex;

void main()
{
    vec3 texColour = texture(tex, data.texCoord).rgb;
    //gouraud uses ambient or only the highlight can be seen
    float ambientLight = 0.2;
    vec3 ambient = ambientLight * lightColour;

    //find the light direction

    float diffuseStrength = 0.9;
    vec3 lightDirection = normalize(data.position - lightPosition);
    float diff = max(dot(normalize(data.norm), lightDirection), 0.0); //dot prod
    vec3 diffuse = (diff * diffuseStrength) * lightColour;


    //specular
    float specularLight = 0.5;
    vec3 viewVec = normalize(data.position - (-viewPosition));
    vec3 reflectionVec = reflect(-lightDirection, normalize(data.norm));

    float specAmt = pow(max(dot(reflectionVec, viewVec), 0.0), 32);
    vec3 specular = specularLight * specAmt * lightColour;

    vec3 result = (ambient + diffuse + specular) * texColour;

    fragColor = vec4(result, 1.0);
}
