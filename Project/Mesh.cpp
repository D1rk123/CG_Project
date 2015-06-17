#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>
#include "Mesh.hpp"
#include "glmToCout.hpp"

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::mat4;
using glm::mat3;
using std::cout;
using std::endl;

void Mesh::draw() {
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

void Mesh::makeMesh(const Geometry& geom) {
    indicesCount = geom.getNumIndices();
    //Generate a VBO and store vertex data in it
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*geom.getNumVertices(), geom.getVertices(), GL_STATIC_DRAW);
    //Generate a IBO and store triangle index data in it
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*geom.getNumIndices(), geom.getIndices(), GL_STATIC_DRAW);

    ellipsoid = geom.approxBoundingEllipsoid();
}

