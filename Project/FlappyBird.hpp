#ifndef FLAPPYBIRD_H
#define FLAPPYBIRD_H
#define _USE_MATH_DEFINES
#include "GameObject.hpp"
#include "Mesh.hpp"
#include "glm/ext.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

class FlappyBird : public GameObject
{
    float gravity;
    float fallVelocity;
    float flyVelocity;
    float maxVelocity;
    float jumpspeed;

    public:
        FlappyBird() : GameObject(){
            gravity = 9.81f;
            fallVelocity = 0.0f;
            flyVelocity = 10.0f;
            maxVelocity = 10000.0f;
            jumpspeed = 10.0f;
        }

        void setFlyVelocity(float vel) {
            flyVelocity = vel;
        }

        float getFlyVelocity() {
            return flyVelocity;
        }

        void startFlying() {
            setMovement(glm::vec3(flyVelocity,fallVelocity,0.0f));
        }

        void increaseJumpVelocity() {
            increaseMovementY(jumpspeed);
            fallVelocity = 0;
        }

        void increaseFallVelocity(float seconds) {
            if (-fallVelocity < maxVelocity) {
                fallVelocity -= seconds*gravity;
                increaseMovementY(-seconds*gravity);
            }
        }

        bool touchingGround(float height) {
            float radiusFlappy = mesh->getEllipsoid().radius;
            return (orientation[3][1] < -height+radiusFlappy);
        }

        void update(float elapsedTime, float height) {
            // update flymovement
            if (orientation[3][1] >= height) {
                setVelocityY(-5.0f);
            }
            increaseFallVelocity(elapsedTime);
            glm::vec3 position = glm::vec3(orientation[3][0], orientation[3][1], orientation[3][2]);
            glm::vec3 update = movement*elapsedTime;

            // Find rotation matrix to make flappybird face in direction of movement
            glm::vec3 dir = glm::normalize(movement);
            glm::vec3 z = glm::vec3(0.0f,0.0f,1.0f);
            glm::vec3 cross = glm::cross(dir,z);
            glm::mat4 flightRotation = glm::mat4(glm::vec4(dir, 0.0f), glm::vec4(z,0.0f), glm::vec4(cross, 0.0f), glm::vec4(0,0,0,1.0f));

            orientation = glm::translate(position + update) * flightRotation;
        }

    protected:
    private:
};

#endif // FLAPPYBIRD_H
