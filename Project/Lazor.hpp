#ifndef LAZOR_H
#define LAZOR_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "GameObject.hpp"
#include "Texture.hpp"

class Lazor: public GameObject
{
    static Texture lazorTextures[3];
    double frame;
    glm::vec3 direction;
    bool isDescending;
    float speed;

    public:
    static void loadLazorTextures() {

    }

    size_t getTextureIndex() {
        return (size_t)frame;
    }

    Lazor(glm::mat4 orientation, glm::vec3 direction, Mesh* mesh) : GameObject(mesh) {
        this->orientation = orientation * glm::rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        this->direction = direction;
        isDescending = true;
        speed = 3.0f;
        frame = 2.0f;
    }

    void update(float time) {
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
    }
};

#endif // LAZOR_H
