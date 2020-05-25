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
    vec4 ambient = vec4(0.5, 0.5, 0.5, 0);
    vec3 globLightPos = vec3(0, 0, 10);
    vec3 globDiffuse = vec3(0.3, 0.3, 0.3);
    vec3 globSpecular = vec3(0.1,0.1,0.1);
    float shininess = 10;
    float shininessFactor = 3.0f;

    vec3 globLightDir = normalize(globLightPos - vert);
    vec3 globEyeDir = normalize(camera[3].xyz-vert); // we are in Eye Coordinates, so EyePos is (0,0,0)
    vec3 globReflection = normalize(-reflect(globLightDir,normal));

    //calculate diffuse term
    vec3 globIdiff = globDiffuse * max(dot(normal,globLightDir), 0.0);
    vec3 diffSum = clamp(globIdiff, 0.0, 1.0);

    // calculate specular term
    vec3 specSum = globSpecular * pow(max(dot(globReflection,globEyeDir),0.0),shininess * shininessFactor);

    vec3 lightPos = lazorPos[0];
    vec3 diffuse = vec3(1.0, 0.2, 0.2);
    vec3 specular = vec3(0.3,0.0,0.0);

    for(int i=0; i<10; i++) {

        vec3 pointToLight = lightPos - vert;
        float lightdistance = length(pointToLight);
        vec3 lightDir = pointToLight / lightdistance;
        vec3 eyeDir = normalize(camera[3].xyz-vert);
        vec3 reflection = normalize(-reflect(lightDir,normal));

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
}
