#include "BoundingSphere.hpp"
#include <glm/gtx/string_cast.hpp>

std::ostream &operator<<(std::ostream &os, BoundingSphere const &sphere) {
    return os << "BoundingSphere(" << glm::to_string(sphere.pos) << ", " << sphere.radius << ")";
}

