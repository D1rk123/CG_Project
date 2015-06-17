#version 330

in vec3 normal;
in vec2 texCoords;
in vec3 vert;

out vec4 colorOut;


uniform sampler2D textureSampler;

void main(void)
{
    vec3 lightPos = vec3(0, 0, 10);
    vec4 diffuse = vec4(0.3, 0.3, 0.3, 1);
    vec4 ambient = vec4(0.05, 0.05, 0.05, 1);
    vec4 specular = vec4(0.5,0.1,0.1,1);
    float shininess = 10;

    vec3 lightDir = normalize(lightPos - vert);
    vec3 eyeDir = normalize(vec3(0, 0, 20)-vert); // we are in Eye Coordinates, so EyePos is (0,0,0)
    vec3 reflection = normalize(-reflect(lightDir,normal));

    float shininessFactor = 3f;

    //calculate diffuse term
    vec4 Idiff = diffuse * max(dot(normal,lightDir), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);

    // calculate specular term
    vec4 Ispec = specular * pow(max(dot(reflection,eyeDir),0.0),shininess * shininessFactor);

    colorOut = ambient + Idiff + Ispec + 0.5 * vec4(texture(textureSampler, texCoords).rgb, 1.0);
    //colorOut = vec4(0.25*(normal+vec3(1)),1) + 0.5 * vec4(texture(textureSampler, texCoords).rgb, 1.0);
}
