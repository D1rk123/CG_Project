#version 420

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
out vec3 color;

void main(void)
{
    gl_Position = vec4(pos, 1.0);
    color = normal;
}
