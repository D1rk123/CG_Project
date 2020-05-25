#ifndef HPP_LASER
#define HPP_LASER
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "GameObject.hpp"
#include "Texture.hpp"

class Laser: public GameObject
{
    double frame;
    glm::vec3 direction;
    bool isDescending;
    float speed;

public:
    Laser(glm::mat4 orientation, glm::vec3 direction, Mesh* mesh) : GameObject() {
        this->orientation = orientation * glm::translate(glm::vec3(1.5f, 0.0f, 0.0f)) * glm::rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        this->direction = direction;
        this->setMesh(mesh);
        isDescending = true;
        speed = 3.0f;
        frame = 2.0f;
    }

    void update(float time, Texture* laserTextures) {
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
};

#endif // LAZOR_H
