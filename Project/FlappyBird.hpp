#ifndef FLAPPYBIRD_H
#define FLAPPYBIRD_H
#define _USE_MATH_DEFINES

#include "Mesh.hpp"
#include "glm/ext.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

class FlappyBird
{
    Mesh* flappyMesh;
    float gravity;
    float fallVelocity;
    float airResistance;
    float flyVelocity;
    //float dragCoefficient = 0.47;
    float maxVelocity;
    float jumpspeed;

    public:
        FlappyBird(Mesh* mesh){
            gravity = 9.81f;
            fallVelocity = 0.0f;
            airResistance = 0.0f;
            flyVelocity = 10.0f;
            maxVelocity = 10000.0f;
            jumpspeed = 10.0f;
            fallVelocity = 0.0f;
            flappyMesh = mesh;
        }
        FlappyBird() {}

        void setFlyVelocity(float vel) {
            flyVelocity = vel;
        }

        float getFlyVelocity() {
            return flyVelocity;
        }

        void setMesh(Mesh* flappy) {
            flappyMesh = flappy;
        }

        Mesh* getMesh() {
            return flappyMesh;
        }

       /* float getAirResistance() {
            return airResistance;
        }

        void updateAirResistance() {
            vertVel = flappyMesh.getVelocityY();
            airResistance = vertVel;
        }

        void setAirResistance(float airRes) {
            airResistance = airRes;
        }*/

        void startFlying() {
            flappyMesh->setMovement(glm::vec3(flyVelocity,fallVelocity,0.0f));
        }


        void increaseJumpVelocity() {
            flappyMesh->increaseMovementY(jumpspeed);
            fallVelocity = 0;
        }

        void increaseFallVelocity(float seconds) {
            if (-fallVelocity < maxVelocity) {
                fallVelocity -= seconds*gravity;
                flappyMesh->increaseMovementY(-seconds*gravity);
            }
        }

        void update(float elapsedTime) {
            // update flymovement
            increaseFallVelocity(elapsedTime);
            glm::vec3 position = glm::vec3(flappyMesh->getOrientation()[3][0], flappyMesh->getOrientation()[3][1], flappyMesh->getOrientation()[3][2]);
            glm::vec3 update = flappyMesh->getMovement()*elapsedTime;

            // Find rotation matrix to make flappybird face in direction of movement
            glm::vec3 dir = glm::normalize(flappyMesh->getMovement());
            glm::vec3 z = glm::vec3(0.0f,0.0f,1.0f);
            glm::vec3 cross = glm::cross(dir,z);
            glm::mat4 flightRotation = glm::mat4(glm::vec4(dir, 0.0f), glm::vec4(z,0.0f), glm::vec4(cross, 0.0f), glm::vec4(0,0,0,1.0f));

            flappyMesh->setOrientation(glm::translate(position + update) * flightRotation);
        }

    protected:
    private:
};

#endif // FLAPPYBIRD_H
