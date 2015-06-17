#version 330

in vec3 normal;
in vec2 texCoords;
in vec3 vert;

out vec4 colorOut;


uniform sampler2D textureSampler;
uniform mat4 camera;
uniform vec3 lazorPos[10];

void main(void)
{
    vec3 lightPos = lazorPos[0];
    vec4 diffuse = vec4(1.0, 1.0, 1.0, 1);
    vec4 ambient = vec4(0.3, 0.3, 0.3, 1);
    vec4 specular = vec4(0.3,0.3,0.3,1);
    float shininess = 10;

    vec3 pointToLight = lightPos - vert;
    float lightdistance = length(pointToLight);
    vec3 lightDir = pointToLight / lightdistance;
    vec3 eyeDir = normalize(camera[3].xyz-vert);
    vec3 reflection = normalize(-reflect(lightDir,normal));

    float shininessFactor = 3.0f;

    //calculate diffuse term
    vec4 Idiff = diffuse * max(dot(normal,lightDir)/lightdistance, 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);

    // calculate specular term
    vec4 Ispec = specular * pow(max(dot(reflection,eyeDir)/lightdistance,0.0),shininess * shininessFactor);

    colorOut = Ispec + (ambient + Idiff) * vec4(texture(textureSampler, texCoords));
    //colorOut = vec4(0.25*(normal+vec3(1)),1) + 0.5 * vec4(texture(textureSampler, texCoords).rgb, 1.0);
}
