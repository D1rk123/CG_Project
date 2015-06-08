#version 420

layout (location = 0) in vec3 normal;
layout (location = 1) in vec2 texCoords;

uniform sampler2D textureSampler;

void main(void)
{
    gl_FragColor = vec4(0.25*(normal+vec3(1)),1) + 0.5 * vec4(texture(textureSampler, texCoords).rgb, 1.0);
}
