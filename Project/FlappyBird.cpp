#include "FlappyBird.hpp"

void FlappyBird::increaseFallVelocity(float seconds, float height) {
    increaseVelocityY(-seconds*gravity);
    if (orientation[3][1] >= 0.7*height) {
        increaseVelocityY(-seconds*outOfScreenSpring*(orientation[3][1] - 0.7*height));
    }
}

void FlappyBird::update(float frameDuration, float gameDuration, float height) {
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

void FlappyBird::startFlying() {
    setVelocity(glm::vec3(startFlyVelocity,0.0f,0.0f));
    orientation[3][1] = 1.5f;
}

void FlappyBird::jump() {
    hasJumped = true;
}

bool FlappyBird::touchingGround(float height) {
    float radiusFlappy = mesh->getEllipsoid().radius;
    return (orientation[3][1] < -height+radiusFlappy);
}

