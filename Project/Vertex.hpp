#ifndef HPP_VERTEX
#define HPP_VERTEX
#include <glm/vec3.hpp>

/**
 @brief Contains the data of one vertex.

 @author Dirk Schut
 @since 4-6-2015
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

#endif //HPP_VERTEX
