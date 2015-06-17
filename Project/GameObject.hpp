#ifndef HPP_GAMEOBJECT
#define HPP_GAMEOBJECT
#include <glm/glm.hpp>
#include "Mesh.hpp"
#include "BoundingEllipsoid.hpp"

class GameObject {
    protected:
    Mesh* mesh;
    glm::mat4 orientation;
    glm::vec3 movement;

    public:
    GameObject() {
    }
    GameObject(Mesh* mesh) {
        this->mesh = mesh;
    }

    bool testCollision(const GameObject& other);

    void transform(const glm::mat4& transformation) {
        orientation = transformation * orientation;
    }
    void setOrientation(const glm::mat4& newOrientation) {
        orientation = newOrientation;
    }
    const glm::mat4& getOrientation() {
        return orientation;
    }
    const Mesh& getMesh() {
        return *mesh;
    }
    void setMesh(Mesh* mesh) {
        this->mesh = mesh;
    }

    /*float getVelocity() {
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
    */
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

#endif // HPP_GAMEOBJECT
