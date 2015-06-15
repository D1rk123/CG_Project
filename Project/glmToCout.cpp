#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "glmToCout.hpp"

std::ostream &operator<<(std::ostream &os, const glm::vec4 vec) {
    return os << glm::to_string(vec);
}
std::ostream &operator<<(std::ostream &os, const glm::vec3 vec) {
    return os << glm::to_string(vec);
}
std::ostream &operator<<(std::ostream &os, const glm::vec2 vec) {
    return os << glm::to_string(vec);
}
std::ostream &operator<<(std::ostream &os, const glm::mat4 mat) {
    return os << glm::to_string(mat);
}
std::ostream &operator<<(std::ostream &os, const glm::mat3 mat) {
    return os << glm::to_string(mat);
}
