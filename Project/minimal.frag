#version 330

in vec2 texCoords;

uniform sampler2D textureSampler;

varying vec3 normalEye;
varying vec3 vert;

void main(void)
{
    vec3 L = normalize(gl_LightSource[0].position.xyz - vert);
    vec3 E = normalize(-vert); // we are in Eye Coordinates, so EyePos is (0,0,0)
    vec3 R = normalize(-reflect(L,normalEye));

    float shininessFactor = 0.3f;
    //calculate ambient term
    vec4 Iamb = gl_FrontLightProduct[0].ambient;

    //calculate diffuse term
    vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(normalEye,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);

    // calculate specular term
    vec4 Ispec = gl_FrontLightProduct[0].specular
                 * pow(max(dot(R,E),0.0),shininessFactor*gl_FrontMaterial.shininess);
    Ispec = clamp(Ispec, 0.0, 1.0);
    // write Total Color:
    gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec + 0.5 * vec4(texture(textureSampler, texCoords).rgb, 1.0);
}
