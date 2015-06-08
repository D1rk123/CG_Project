#ifndef HPP_MESH
#define HPP_MESH
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include "Vertex.hpp"

using glm::vec3;

/**
 @brief Class for loading Mesh data into the video card
*/
class Mesh {
    GLuint vbo, ibo;
    GLsizei indicesCount;

    public:
    GLuint getVBO() {
        return vbo;
    }
    GLuint getIBO() {
        return ibo;
    }

    GLsizei getIndicesCount() {
        return indicesCount;
    }

    void makeCube(glm::vec3 lower, glm::vec3 upper)
    {
        const GLfloat d = 0.57735026918962576; // 1/sqrt(3), (d, d, d) is a vector with length 1
        Vertex cubeCorners[] = {
            Vertex(glm::vec3(lower[0], lower[1], lower[2]), glm::vec3(-d, -d, -d), glm::vec2(0.0, 0.0)),
            Vertex(glm::vec3(lower[0], upper[1], lower[2]), glm::vec3(-d,  d, -d), glm::vec2(0.0, 1.0)),
            Vertex(glm::vec3(upper[0], lower[1], lower[2]), glm::vec3( d, -d, -d), glm::vec2(1.0, 0.0)),
            Vertex(glm::vec3(upper[0], upper[1], lower[2]), glm::vec3( d,  d, -d), glm::vec2(1.0, 1.0)),
            Vertex(glm::vec3(lower[0], lower[1], upper[2]), glm::vec3(-d, -d,  d), glm::vec2(1.0, 0.0)),
            Vertex(glm::vec3(lower[0], upper[1], upper[2]), glm::vec3(-d,  d,  d), glm::vec2(1.0, 1.0)),
            Vertex(glm::vec3(upper[0], lower[1], upper[2]), glm::vec3( d, -d,  d), glm::vec2(0.0, 0.0)),
            Vertex(glm::vec3(upper[0], upper[1], upper[2]), glm::vec3( d,  d,  d), glm::vec2(0.0, 1.0))
        };
        unsigned int cubeIndices[] = {
            2, 1, 0, //back side
            1, 2, 3,
            4, 5, 6, //front side
            7, 6, 5,
            0, 1, 5, //left side
            0, 5, 4,
            7, 3, 2, //right side
            6, 7, 2,
            7, 5, 1, //top side
            3, 7, 1,
            0, 4, 6, //bottom side
            0, 6, 2
        };
        indicesCount = 36;

        //Generate a VBO and store vertex data in it
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeCorners), cubeCorners, GL_STATIC_DRAW);
        //Generate a IBO and store triangle index data in it
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
    }
    void removeBuffers()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }
};

#endif //HPP_MESH
