#ifndef HPP_VERTEX
#define HPP_VERTEX
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

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

std::ostream &operator<<(std::ostream &os, Vertex const &vert);

#endif //HPP_VERTEX
