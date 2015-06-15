#ifndef HPP_BOUNDINGELLIPSOID
#define HPP_BOUNDINGELLIPSOID
#include <iostream>
#include "glm/glm.hpp"

struct BoundingEllipsoid {
    glm::mat4 orientation;
    float radius;
    BoundingEllipsoid() {

    }
    BoundingEllipsoid(glm::mat4 orientation, float radius) : orientation(orientation), radius(radius) {

    }
};

std::ostream &operator<<(std::ostream &os, BoundingEllipsoid const &ellip);

#endif // HPP_BOUNDINGELLIPSOID

