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

    void update(float time, Texture* laserTextures);
};

#endif // LAZOR_H
