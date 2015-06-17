#ifndef LAZOR_H
#define LAZOR_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

struct Lazor
{
    glm::mat4 orientation;
    double frame;
    glm::vec3 direction;
    bool isDescending;
    float speed;


    Lazor() {
        isDescending = true;
    }

    Lazor(glm::mat4 orientation, glm::vec3 direction) {
        this->orientation = orientation;
        this->direction = direction;
        isDescending = true;
        speed = 10;
    }

    void update(float time) {
        // Laser animation
        if(!isDescending) {
            if(frame <= 3) {
                frame += 0.01;
            }else{
                isDescending = true;
            }
        }

        if (isDescending)
        {
            if(frame >= 0){
                frame -= 0.01;
            }else{
                isDescending = false;
            }
        }

        orientation = glm::translate(direction*time*speed)*orientation;
    }
};

#endif // LAZOR_H
