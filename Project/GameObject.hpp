#ifndef HPP_GAMEOBJECT
#define HPP_GAMEOBJECT
#include <glm/glm.hpp>
#include "Mesh.hpp"
#include "BoundingEllipsoid.hpp"
#include "glmToCout.hpp"

using std::cout;
using std::endl;

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
    //Only works for objects completely before the near clipping plane
    //Which is all object in this games
    bool checkOutsideOfView (const glm::mat4& camera, float margin) {
        float stopValue = 1.0f + margin;
        float radius = mesh->getEllipsoid().radius;
        glm::vec4 centerPos = orientation[3];

        glm::vec4 downLeftBound = camera * centerPos + glm::vec4(-radius, -radius, radius, 0.0f);
        glm::vec4 upRightBound = camera * centerPos + glm::vec4(radius, radius, radius, 0.0f);

        if(downLeftBound.x / downLeftBound.w > stopValue || downLeftBound.y / downLeftBound.w > stopValue)
            return true;
        if(upRightBound.x / upRightBound.w < -stopValue || upRightBound.y / upRightBound.w < -stopValue)
            return true;
        return false;
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

};

#endif // HPP_GAMEOBJECT
