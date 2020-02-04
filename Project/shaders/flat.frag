#version 330

in vec2 texCoords;

out vec4 colorOut;

uniform sampler2D textureSampler;

void main(void)
{
    colorOut = texture(textureSampler, texCoords).rgba;
}
