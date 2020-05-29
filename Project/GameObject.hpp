#ifndef HPP_GAMEOBJECT
#define HPP_GAMEOBJECT
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.hpp"
#include "Texture.hpp"
#include "BoundingEllipsoid.hpp"
#include "glmToCout.hpp"

using std::cout;
using std::endl;

class GameObject {
    protected:
    Mesh* mesh;
    Texture* texture;
    glm::mat4 orientation;
    glm::vec3 velocity;
    bool collided;

    public:
    GameObject() {
        collided = false;
    }
    GameObject(Mesh* mesh, Texture* texture) {
        this->mesh = mesh;
        this->texture = texture;
        collided = false;
    }
    //Only works for objects completely before the near clipping plane
    //Which is all objects in this game
    bool checkOutsideOfView (const glm::mat4& camera, float margin);

    //Only works for objects completely before the near clipping plane
    //Which is all objects in this game
    bool checkOutsideOfView (const glm::mat4& camera);

    bool checkAwayFromFlappy (float flappyPosX) {
        if(orientation[3][0] < flappyPosX - 40.0f) {
            return true;
        } else {
            return false;
        }
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
    void setTexture(Texture* texture) {
        this->texture = texture;
    }

    bool getCollided() {
        return collided;
    }

    void setCollided(bool collided) {
        this->collided = collided;
    }

    glm::vec3 getVelocity() {
        return velocity;
    }

    void setVelocity(glm::vec3 newV) {
        velocity = newV;
    }

    void increaseVelocityX(float plusVX) {
        velocity += glm::vec3(plusVX, 0.0f, 0.0f);
    }

    void increaseVelocityY(float plusVY) {
        velocity += glm::vec3(0.0f, plusVY, 0.0f);
    }

    void increaseVelocity(glm::vec3 plusV) {
        velocity += plusV;
    }

    void setVelocityY(float velY) {
        velocity[1] = velY;
    }

    void draw(GLuint orientationMatrixLocation);

};

#endif // HPP_GAMEOBJECT
