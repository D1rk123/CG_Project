#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 texCoordsIn;

out vec2 texCoords;

uniform float offset;

uniform mat4 camera;
uniform mat4 orientation;

void main(void)
{
    gl_Position = camera * orientation * vec4(pos, 1.0f);

    texCoords = texCoordsIn;
}

