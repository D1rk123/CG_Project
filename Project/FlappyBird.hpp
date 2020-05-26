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
    float outOfScreenSpring;
    float startFlyVelocity;
    float flyAcceleration;
    float maxYVelocity;
    float jumpSpeed;
    bool hasJumped;

    public:
        FlappyBird() : GameObject(){
            gravity = 9.81f;
            outOfScreenSpring = 20.0f;
            startFlyVelocity = 10.0f;
            flyAcceleration = 0.15f;
            maxYVelocity = 25.0f;
            jumpSpeed = 10.0f;
            hasJumped = false;
        }

        float getJumpSpeed() {
            return jumpSpeed;
        }

        void startFlying() {
            setVelocity(glm::vec3(startFlyVelocity,0.0f,0.0f));
            orientation[3][1] = 1.5f;
        }

        void jump() {
            hasJumped = true;
        }

        void increaseFallVelocity(float seconds, float height) {
            increaseVelocityY(-seconds*gravity);
            if (orientation[3][1] >= 0.7*height) {
                increaseVelocityY(-seconds*outOfScreenSpring*(orientation[3][1] - 0.7*height));
            }
        }

        bool touchingGround(float height) {
            float radiusFlappy = mesh->getEllipsoid().radius;
            return (orientation[3][1] < -height+radiusFlappy);
        }

        void update(float frameDuration, float gameDuration, float height) {
            velocity[0] = startFlyVelocity + gameDuration * flyAcceleration;

            if(hasJumped) {
                increaseVelocityY(jumpSpeed);
                hasJumped = false;
            }
            increaseFallVelocity(frameDuration, height);
            glm::vec3 position = glm::vec3(orientation[3][0], orientation[3][1], orientation[3][2]);
            glm::vec3 update = velocity*frameDuration;
            if(velocity[1] > maxYVelocity) {
                velocity[1] = maxYVelocity;
            } else if (velocity[1] < -maxYVelocity) {
                velocity[1] = -maxYVelocity;
            }

            // Find rotation matrix to make flappybird face in direction of movement
            glm::vec3 dir = glm::normalize(velocity);
            glm::vec3 z = glm::vec3(0.0f,0.0f,1.0f);
            glm::vec3 cross = glm::cross(dir,z);
            glm::mat4 flightRotation = glm::mat4(glm::vec4(dir, 0.0f), glm::vec4(z,0.0f), glm::vec4(cross, 0.0f), glm::vec4(0,0,0,1.0f));

            orientation = glm::translate(position + update) * flightRotation;
        }

    protected:
    private:
};

#endif // FLAPPYBIRD_H
