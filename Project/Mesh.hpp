#ifndef HPP_MESH
#define HPP_MESH
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <stdlib.h>
#include <math.h>
#include "Vertex.hpp"

const double pi = 3.141592653589793238462643383279502884;

using glm::vec3;
using glm::vec2;

/**
 @brief Class for loading Mesh data into the video card
*/
class Mesh {
    GLuint vbo, ibo;
    GLsizei indicesCount;

    public:
    GLuint getVBO() {
        return vbo;
    }
    GLuint getIBO() {
        return ibo;
    }

    GLsizei getIndicesCount() {
        return indicesCount;
    }

    void makeCube(glm::vec3 lower, glm::vec3 upper)
    {
        const GLfloat d = 0.57735026918962576; // 1/sqrt(3), (d, d, d) is a vector with length 1
        Vertex cubeCorners[] = {
            Vertex(glm::vec3(lower[0], lower[1], lower[2]), glm::vec3(-d, -d, -d), glm::vec2(0.0, 0.0)),
            Vertex(glm::vec3(lower[0], upper[1], lower[2]), glm::vec3(-d,  d, -d), glm::vec2(0.0, 1.0)),
            Vertex(glm::vec3(upper[0], lower[1], lower[2]), glm::vec3( d, -d, -d), glm::vec2(1.0, 0.0)),
            Vertex(glm::vec3(upper[0], upper[1], lower[2]), glm::vec3( d,  d, -d), glm::vec2(1.0, 1.0)),
            Vertex(glm::vec3(lower[0], lower[1], upper[2]), glm::vec3(-d, -d,  d), glm::vec2(1.0, 0.0)),
            Vertex(glm::vec3(lower[0], upper[1], upper[2]), glm::vec3(-d,  d,  d), glm::vec2(1.0, 1.0)),
            Vertex(glm::vec3(upper[0], lower[1], upper[2]), glm::vec3( d, -d,  d), glm::vec2(0.0, 0.0)),
            Vertex(glm::vec3(upper[0], upper[1], upper[2]), glm::vec3( d,  d,  d), glm::vec2(0.0, 1.0))
        };
        unsigned int cubeIndices[] = {
            2, 1, 0, //back side
            1, 2, 3,
            4, 5, 6, //front side
            7, 6, 5,
            0, 1, 5, //left side
            0, 5, 4,
            7, 3, 2, //right side
            6, 7, 2,
            7, 5, 1, //top side
            3, 7, 1,
            0, 4, 6, //bottom side
            0, 6, 2
        };
        indicesCount = 36;

        //Generate a VBO and store vertex data in it
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeCorners), cubeCorners, GL_STATIC_DRAW);
        //Generate a IBO and store triangle index data in it
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
    }

    void makeRandomMeteor(int segments, int rings)
    {
        assert(segments >= 3);
        assert(rings >= 2);

        int vertexAmount = (rings-1)*segments+2;
        indicesCount = 3*(rings-1)*segments*2;
        Vertex meteorCorners[vertexAmount];
        unsigned int meteorIndices[indicesCount];

        //Calculate vertex positions
        for(int i=1; i<rings; i++) {
            double theta = (double(i)/rings)*pi;
            double sinTheta = sin(theta);
            double cosTheta = cos(theta);
            for(int j=0; j<segments; j++) {
                double phi = (double(j)/segments)*2*pi;
                double sinPhi = sin(phi);
                double cosPhi = cos(phi);
                vec3 position = vec3(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
                //std::cout << glm::to_string(position) << std::endl;
                meteorCorners[(i-1)*segments+j] = Vertex(position, position, vec2(0,0));
            }
        }

        //Calculate upper and lower ring faces
        int topCorner = (rings-1)*segments;
        int bottomCorner = topCorner+1;
        meteorCorners[topCorner] = Vertex(vec3(0,0,1), vec3(0,0,1), vec2(0,0));
        meteorCorners[bottomCorner] = Vertex(vec3(0,0,-1), vec3(0,0,-1), vec2(0,0));
        for(int i=0; i<segments; i++) {
            meteorIndices[i*3] = topCorner;
            meteorIndices[i*3+1] = i;
            meteorIndices[i*3+2] = (i+1)%segments;
            meteorIndices[3*segments+i*3] = bottomCorner;
            meteorIndices[3*segments+i*3+1] = (rings-2)*segments+i;
            meteorIndices[3*segments+i*3+2] = (rings-2)*segments+((i+1)%segments);
        }

        //calculate faces of other rings
        for(int i=0; i<rings-2; i++) {
            int indexOffset = (i+1)*(6*segments);
            for(int j=0; j<segments; j++) {
                meteorIndices[indexOffset+j*6+0] = i*segments+j;
                meteorIndices[indexOffset+j*6+1] = i*segments+(j+1)%segments;
                meteorIndices[indexOffset+j*6+2] = (i+1)*segments+(j+1)%segments;
                meteorIndices[indexOffset+j*6+3] = i*segments+j;
                meteorIndices[indexOffset+j*6+4] = (i+1)*segments+(j+1)%segments;
                meteorIndices[indexOffset+j*6+5] = (i+1)*segments+j;
            }
        }


        /*for(int i=0; i<vertexAmount; i++) {
            std::cout << i << ": " << meteorCorners[i] << std::endl;
        }
        for(int i=0; i<indicesCount/3; i++) {
            std::cout << i << ": " << meteorIndices[i*3] << "/" << meteorIndices[i*3+1] << "/" << meteorIndices[i*3+2] << std::endl;
        }*/

        //Generate a VBO and store vertex data in it
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexAmount, meteorCorners, GL_STATIC_DRAW);
        //Generate a IBO and store triangle index data in it
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indicesCount, meteorIndices, GL_STATIC_DRAW);
    }
    void removeBuffers()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }
};

#endif //HPP_MESH
