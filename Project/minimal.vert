#version 420

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout (location = 0) out vec3 normalOut;
layout (location = 1) out vec2 texCoordsOut;

uniform mat4 camera;

void main(void)
{
    gl_Position = camera*vec4(pos, 1.0);
    //map each normal to a color and send it to the fragment shader
    //color = (normal+vec3(1))/2.0;
    normalOut = normal;
    texCoordsOut = texCoords;
}
