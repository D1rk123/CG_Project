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
    float energy;
    float airResistance;
    float flyVelocity;
    //float dragCoefficient = 0.47;
    float maxVelocity;
    float jumpspeed;

    public:
        FlappyBird(Mesh* mesh){
            gravity = 9.81f;
            fallVelocity = 0.0f;
            energy = 1.0f;
            airResistance = 0.0f;
            flyVelocity = 10.0f;
            maxVelocity = 1000.0f;
            jumpspeed = 10.0f;
            fallVelocity = 0.0f;
            flappyMesh = mesh;
        }
        FlappyBird() {}

        void setFlyVelocity(float vel) {
            flyVelocity = vel;
        }

        void changeEnergy(float change) {
            if (energy+change < 1.0 && energy+change > 0.0) {
                energy += change;
            }
        }

        float getEnergy() {
            return energy;
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
            if (energy > 0) {
                flappyMesh->increaseMovementY(jumpspeed*energy);
            }
        }

        void increaseFallVelocity(float seconds) {
            if (-fallVelocity < maxVelocity) {
                fallVelocity -= seconds*gravity;
                flappyMesh->increaseMovementY(-seconds*gravity);
            }
        }

    protected:
    private:
};

#endif // FLAPPYBIRD_H
