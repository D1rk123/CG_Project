#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 texCoordsIn;

out vec3 normal;
out vec2 texCoords;

uniform mat4 camera;
uniform mat4 orientation;

void main(void)
{
    gl_Position = camera*orientation*vec4(pos, 1.0);
    //map each normal to a color and send it to the fragment shader
    //color = (normal+vec3(1))/2.0;
    mat3 normalRotationMatrix = mat3(orientation[0].xyz, orientation[1].xyz, orientation[2].xyz);
    normal = normalRotationMatrix*normalIn;
    texCoords = texCoordsIn;
}
