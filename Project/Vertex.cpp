#include "Vertex.hpp"

std::ostream &operator<<(std::ostream &os, Vertex const &vert) {
    return os << "Vertex(" << glm::to_string(vert.pos) << ", " << glm::to_string(vert.normal) << ", " << glm::to_string(vert.tex) << ")";
}
