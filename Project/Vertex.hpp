#ifndef HPP_VERTEX
#define HPP_VERTEX
#include <iostream>
#include <glm/glm.hpp>

/**
 @brief Contains the data of one vertex.
*/
struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex;

    Vertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& tex) : pos(pos), normal(normal), tex(tex)
    {
    }

    Vertex()
    {
    }

};

std::ostream &operator<<(std::ostream &os, Vertex const &vert);

#endif //HPP_VERTEX
