#include "Laser.hpp"


void Laser::update(float time, Texture* laserTextures) {
    // Laser animation
    if(!isDescending) {
        frame += time * 10.0f;
        if(frame > 3.0f) {
            isDescending = true;
            frame = 2.99;
        }
    } else {
        frame -= time * 10.0f;
        if(frame < 0){
            isDescending = false;
            frame = 0.0f;
        }
    }
    orientation = glm::translate(direction*time*speed) * orientation;
    texture = &laserTextures[(size_t)frame];
}
