#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 texCoordsIn;

out vec3 normal;
out vec2 texCoords;
out vec3 vert;

uniform mat4 camera;
uniform mat4 orientation;

void main(void)
{
    vec4 vertexPos = orientation*vec4(pos,1.0);
    gl_Position = camera*vertexPos;
    vert = vertexPos.xyz;

    mat3 normalRotationMatrix = mat3(orientation[0].xyz, orientation[1].xyz, orientation[2].xyz);
    normal = normalRotationMatrix*normalIn;

    texCoords = texCoordsIn;
}
