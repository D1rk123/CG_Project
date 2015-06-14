#ifndef HPP_BOUNDINGSPHERE
#define HPP_BOUNDINGSPHERE
#include <iostream>
#include "glm/vec3.hpp"

struct BoundingSphere {
    glm::vec3 pos;
    float radius;
    BoundingSphere() {

    }
    BoundingSphere(glm::vec3 pos, float radius) : pos(pos), radius(radius) {

    }
};

std::ostream &operator<<(std::ostream &os, BoundingSphere const &sphere);

#endif // HPP_BOUNDINGSPHERE
