#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 texCoordsIn;

out vec3 normal;
out vec2 texCoords;

uniform mat4 camera;

void main(void)
{
    gl_Position = camera*vec4(pos, 1.0);
    //map each normal to a color and send it to the fragment shader
    //color = (normal+vec3(1))/2.0;
    normal = normalIn;
    texCoords = texCoordsIn;
}
