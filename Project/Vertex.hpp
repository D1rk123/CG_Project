#ifndef HPP_VERTEX
#define HPP_VERTEX
#include <glm/vec3.hpp>
#include <glm/vec3.hpp>
#include <glm/ext.hpp>

/**
 @brief Contains the data of one vertex.
*/
struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex;

    Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 tex) : pos(pos), normal(normal), tex(tex)
    {
    }

    Vertex()
    {
    }

};

std::ostream &operator<<(std::ostream &os, Vertex const &vert) {
    return os << "Vertex(" << glm::to_string(vert.pos) << ", " << glm::to_string(vert.normal) << ", " << glm::to_string(vert.tex) << ")";
}

#endif //HPP_VERTEX
