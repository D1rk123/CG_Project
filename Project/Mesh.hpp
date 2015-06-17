#ifndef HPP_MESH
#define HPP_MESH
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
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
    GLuint vbo, ibo;
    GLsizei indicesCount;
    BoundingEllipsoid ellipsoid;

    public:
    Mesh(){
    }
    Mesh(const Geometry& geom){
        makeMesh(geom);
    }

    const BoundingEllipsoid& getEllipsoid() {
        return ellipsoid;
    }

    void draw();

    void makeMesh(const Geometry& geom);

    void remove() {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }
};

#endif //HPP_MESH
