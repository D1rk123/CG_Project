#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 texCoordsIn;

out vec2 texCoords;

uniform mat4 camera;
uniform mat4 orientation;

varying vec3 normalEye;
varying vec3 vert;

void main(void)
{
    mat3 normalRotationMatrix = mat3(orientation[0].xyz, orientation[1].xyz, orientation[2].xyz);

    vert = vec3(orientation * vec4(pos,1.0f)); //Transform vertex to eye space
    normalEye = normalRotationMatrix*normalIn; //Transform normal vector to eye space
    texCoords = texCoordsIn;

    gl_Position = camera*orientation*vec4(vert,1.0);

}
