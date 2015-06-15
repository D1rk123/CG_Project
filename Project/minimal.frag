#version 330

in vec3 normal;
in vec2 texCoords;


uniform sampler2D textureSampler;

void main(void)
{
    gl_FragColor = vec4(0.25*(normal+vec3(1)),1) + 0.5 * vec4(texture(textureSampler, texCoords).rgb, 1.0);
}
