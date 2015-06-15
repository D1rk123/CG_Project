#include "BoundingEllipsoid.hpp"
#include <glm/gtx/string_cast.hpp>

std::ostream &operator<<(std::ostream &os, BoundingEllipsoid const &ellip) {
    return os << "BoundingEllipsoid(" << glm::to_string(ellip.orientation) << ", " << ellip.radius << ")";
}


