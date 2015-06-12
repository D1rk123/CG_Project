#ifndef HPP_MESH
#define HPP_MESH
#include <GL/gl.h>
#include <stdlib.h>
#include <vector>
#include <cstdio>
#include <math.h>
#include "Vertex.hpp"
#include "Geometry.hpp"

/**
 @brief Class for loading and accessing geometry in OpenGL
*/
class Mesh {
    GLuint vbo, ibo, orientationMatrixLocation;
    GLsizei indicesCount;
    glm::mat4 orientation;

    public:
    Mesh(){
        orientation = glm::mat4(1.0f);
    }

    GLuint getVBO() {
        return vbo;
    }
    GLuint getIBO() {
        return ibo;
    }

    GLsizei getIndicesCount() {
        return indicesCount;
    }
    void transform(const glm::mat4& transformation) {
        orientation = orientation * transformation;
    }
    void setOrientation(const glm::mat4& newOrientation) {
        orientation = newOrientation;
    }


    void draw() {
        glm::mat4 invertedOrientation = glm::inverse(orientation);
        //send the orientation matrix to the shader
        glUniformMatrix4fv(orientationMatrixLocation, 1, GL_FALSE, glm::value_ptr(invertedOrientation));
        //Select the mesh vertices
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //select the vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        //select the normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
        //select the texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)*2));

        //select the elements
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        //draw them as triangles
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

        //disable the vertex attribute arrays, because the next shader we use might not use them
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }

    void makeMesh(const Geometry& geom, GLuint orientationMatrixLocation) {
        indicesCount = geom.getNumIndices();
        //Generate a VBO and store vertex data in it
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*geom.getNumVertices(), geom.getVertices(), GL_STATIC_DRAW);
        //Generate a IBO and store triangle index data in it
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*geom.getNumIndices(), geom.getIndices(), GL_STATIC_DRAW);

        this->orientationMatrixLocation = orientationMatrixLocation;
    }

    void remove() {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }
    ~Mesh() {
        remove();
    }
};

#endif //HPP_MESH
