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

    //Only works for objects completely before the near clipping plane
    //Which is all objects in this game
    bool checkOutsideOfView (const glm::mat4& camera) {
        float radius = mesh->getEllipsoid().radius;
        glm::vec4 centerPos = orientation[3];

        glm::vec4 downLeftBound = camera * centerPos + glm::vec4(-radius, -radius, radius, 0.0f);
        glm::vec4 upRightBound = camera * centerPos + glm::vec4(radius, radius, radius, 0.0f);

        //cout << centerPos << endl << downLeftBound << endl << upRightBound << endl;
        if(downLeftBound.x / downLeftBound.w > 1.0f || downLeftBound.y / downLeftBound.w > 1.0f)
            return true;
        if(upRightBound.x / upRightBound.w < -1.0f || upRightBound.y / upRightBound.w < -1.0f)
            return true;
        return false;
    }

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

    void draw(GLuint orientationMatrixLocation) {
        // Load texture at frame
        glBindTexture( GL_TEXTURE_2D, texture->getName() );

        //send the orientation matrix to the shader
        glUniformMatrix4fv(orientationMatrixLocation, 1, GL_FALSE, glm::value_ptr(orientation));

        mesh->draw();
    }

};

#endif // HPP_GAMEOBJECT
