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
#include "BoundingEllipsoid.hpp"

/**
 @brief Class for loading and accessing geometry in OpenGL
*/
class Mesh {
    GLuint vbo, ibo, orientationMatrixLocation;
    GLsizei indicesCount;
    glm::mat4 orientation;
    BoundingEllipsoid ellip;
    glm::vec3 movement;

    public:
    Mesh(){
        orientation = glm::mat4(1.0f);
        //movement = glm:vec4(glm::vec3(0.0f), 1.0f);
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

    float getVelocity() {
        return glm::length(movement);
    }

    float getVelocityX() {
        return movement[0];
    }

    float getVelocityY() {
        return movement[1];
    }

    void setVelocityX(float xVel) {
        glm::vec3 addvec = glm::vec3(xVel, glm::vec2(0.0f));
        movement += addvec;
    }

    void setVelocityY(float yVel) {
        glm::vec3 addvec = glm::vec3(0.0f, yVel, 0.0f);
        movement += addvec;
    }

    glm::vec3 getMovement() {
        return movement;
    }

    void setMovement(glm::vec3 newMove) {
        movement = newMove;
    }

    void increaseMovementX(float plusMovementX) {
        movement += glm::vec3(plusMovementX, 0.0f, 0.0f);
    }

    void increaseMovementY(float plusMovementY) {
        movement += glm::vec3(0.0f, plusMovementY, 0.0f);
    }

    void increaseMovement(glm::vec3 plusMovement) {
        movement += plusMovement;
    }

};

#endif //HPP_MESH
