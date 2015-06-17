#version 330

in vec2 texCoords;

out vec4 colorOut;

uniform sampler2D textureSampler;

void main(void)
{
    //colorOut = 0.5f * vec4(1.0f, 1.0f, 1.0f, 1.0) + 0.5f * vec4(texture(textureSampler, texCoords));
    colorOut = vec4(texture(textureSampler, texCoords).rgb, 1.0);
}
