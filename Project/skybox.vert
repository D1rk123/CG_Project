#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 texCoordsIn;

out vec2 texCoords;

uniform float offset;

void main(void)
{
    gl_Position = vec4(pos, 1.0f);

    texCoords = vec2(texCoordsIn.x/2.0-offset/5.0f, texCoordsIn.y);
}

