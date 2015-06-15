#ifndef HPP_MESH
#define HPP_MESH
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <vector>
#include <cstdio>
#include <math.h>
#include "Vertex.hpp"
#include "Geometry.hpp"
#include "BoundingEllipsoid.hpp"

/**
 @brief Class for loading and accessing geometry in OpenGL
*/
class Mesh {
    GLuint vbo, ibo, orientationMatrixLocation;
    GLsizei indicesCount;
    glm::mat4 orientation;
    BoundingEllipsoid ellip;

    public:
    Mesh(){
        orientation = glm::mat4(1.0f);
    }
    Mesh(const Geometry& geom){
        makeMesh(geom);
    }

    GLuint getVBO() {
        return vbo;
    }
    GLuint getIBO() {
        return ibo;
    }
    const BoundingEllipsoid& getEllip() {
        return ellip;
    }

    GLsizei getIndicesCount() {
        return indicesCount;
    }
    void transform(const glm::mat4& transformation) {
        orientation = transformation * orientation;
    }
    void setOrientation(const glm::mat4& newOrientation) {
        orientation = newOrientation;
    }
    const glm::mat4& getOrientation() {
        return orientation;
    }


    void draw();

    void makeMesh(const Geometry& geom);

    void remove() {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }

    bool testCollision(const Mesh& other);
};

#endif //HPP_MESH
