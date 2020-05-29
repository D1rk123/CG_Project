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

    void increaseFallVelocity(float seconds, float height);

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

    void startFlying();

    void jump();

    bool touchingGround(float height);

    void update(float frameDuration, float gameDuration, float height);
};

#endif // FLAPPYBIRD_H
