#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include "Vertex.hpp"

class Mesh {
    GLuint vbo, ibo;

    public:
    GLuint getVBO() {
        return vbo;
    }
    GLuint getIBO() {
        return ibo;
    }

    GLsizei getIndicesCount() {
        return 36;
    }

    void makeBuffers()
    {
        Vertex cubeCorners[] = {
            Vertex(glm::vec3(0.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(0.0f, 0.5f, 0.0f)),
            Vertex(glm::vec3(0.5f, 0.0f, 0.0f)),
            Vertex(glm::vec3(0.5f, 0.5f, 0.0f)),
            Vertex(glm::vec3(0.0f, 0.0f, 0.5f)),
            Vertex(glm::vec3(0.0f, 0.5f, 0.5f)),
            Vertex(glm::vec3(0.5f, 0.0f, 0.5f)),
            Vertex(glm::vec3(0.5f, 0.5f, 0.5f))
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
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeCorners), cubeCorners, GL_STATIC_DRAW);
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
