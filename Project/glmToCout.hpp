#ifndef HPP_GLMTOCOUT
#define HPP_GLMTOCOUT

#include <glm/glm.hpp>

std::ostream &operator<<(std::ostream &os, const glm::vec4 vec);
std::ostream &operator<<(std::ostream &os, const glm::vec3 vec);
std::ostream &operator<<(std::ostream &os, const glm::vec2 vec);
std::ostream &operator<<(std::ostream &os, const glm::mat4 mat);
std::ostream &operator<<(std::ostream &os, const glm::mat3 mat);


#endif // HPP_GLMTOCOUT
