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

    vec4 ambient = vec4(0.35, 0.35, 0.35, 1);
    vec3 lightPos = lazorPos[0];
    vec3 diffuse = vec3(1.0, 0.5, 0.5);
    vec3 specular = vec3(0.3,0.1,0.1);

    vec3 specSum = vec3(0.0f);
    vec3 diffSum = vec3(0.0f);

    for(int i=0; i<10; i++) {

        float shininess = 10;

        vec3 pointToLight = lightPos - vert;
        float lightdistance = length(pointToLight);
        vec3 lightDir = pointToLight / lightdistance;
        vec3 eyeDir = normalize(camera[3].xyz-vert);
        vec3 reflection = normalize(-reflect(lightDir,normal));

        float shininessFactor = 3.0f;

        //calculate diffuse term
        vec3 Idiff = diffuse * max(dot(normal,lightDir)/lightdistance, 0.0);
        Idiff = clamp(Idiff, 0.0, 0.6);
        diffSum += Idiff;

        // calculate specular term
        vec3 Ispec = specular * pow(max(dot(reflection,eyeDir)/lightdistance,0.0),shininess * shininessFactor);
        Ispec = clamp(Ispec, 0.0, 0.6);
        specSum += Ispec;
    }

    diffSum = clamp(diffSum, 0.0, 1.0);

    colorOut = vec4(specSum, 1.0f) + (ambient + vec4(diffSum, 1.0)) * vec4(texture(textureSampler, texCoords));
    //colorOut = vec4(0.25*(normal+vec3(1)),1) + 0.5 * vec4(texture(textureSampler, texCoords).rgb, 1.0);
    //colorOut = vec4(vec3(texture(textureSampler, texCoords).r/lightdistance), 1.0f);
}
