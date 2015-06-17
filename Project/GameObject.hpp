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
    bool collided;

    public:
    GameObject() {
        collided = false;
    }
    GameObject(Mesh* mesh) {
        this->mesh = mesh;
        collided = false;
    }

    bool testCollision(GameObject* other);

    void transform(const glm::mat4& transformation) {
        orientation = transformation * orientation;
    }
    void setOrientation(const glm::mat4& newOrientation) {
        orientation = newOrientation;
    }
    const glm::mat4& getOrientation() const{
        return orientation;
    }
    const BoundingEllipsoid& getEllipsoid() const {
        return mesh->getEllipsoid();
    }
    const Mesh& getMesh() const {
        return *mesh;
    }
    Mesh& getMesh(){
        return *mesh;
    }
    void setMesh(Mesh* mesh) {
        this->mesh = mesh;
    }

    bool getCollided() {
        return collided;
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

    void setVelocityY(float velY) {
        movement[1] = velY;
    }

};

#endif // HPP_GAMEOBJECT
