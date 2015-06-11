#ifndef HPP_MESH
#define HPP_MESH
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/random.hpp>
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

    vec3 calcTranformedPos(vec3 pos, vec3* directions, int numDirections, float noiseLength) {
        float length = 0.5;
        for(int k=0; k<numDirections; k++) {
            float dot = std::max(glm::dot(pos, directions[k]), 0.0f);
            length += dot*dot*dot*0.5;
        }
        return pos*length+glm::ballRand(noiseLength);
    }

    void makeRandomMeteor(int numSeg, int numRing, int numDirections, float noiseLength)
    {
        assert(numSeg >= 3);
        assert(numRing >= 2);

        vec3 directions[numDirections];

        for(int i=0; i<numDirections; i++) {
            directions[i] = glm::ballRand(1.0f);
            //std::cout << glm::to_string(directions[i]) << std::endl;
        }

        int vertexAmount = (numRing-1)*numSeg+2;
        indicesCount = 3*(numRing-1)*numSeg*2;
        Vertex meteorCorners[vertexAmount];
        unsigned int meteorIndices[indicesCount];

        //Calculate vertex positions
        for(int i=1; i<numRing; i++) {
            double theta = (double(i)/numRing)*pi;
            double sinTheta = sin(theta);
            double cosTheta = cos(theta);
            for(int j=0; j<numSeg; j++) {
                double phi = (double(j)/(numSeg-1))*2*pi;
                double sinPhi = sin(phi);
                double cosPhi = cos(phi);
                vec3 position = vec3(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
                meteorCorners[(i-1)*numSeg+j] = Vertex(calcTranformedPos(position, directions, numDirections, noiseLength), position, vec2(double(j)/numSeg, double(i)/numRing));
            }
        }

        //Calculate upper and lower ring faces
        int topCorner = (numRing-1)*numSeg;
        int bottomCorner = topCorner+1;

        meteorCorners[topCorner] = Vertex(calcTranformedPos(vec3(0,0,1), directions, numDirections, noiseLength), vec3(0,0,1), vec2(0.5f,1.0f));
        meteorCorners[bottomCorner] = Vertex(calcTranformedPos(vec3(0,0,-1), directions, numDirections, noiseLength), vec3(0,0,-1), vec2(0.5f,0.0f));
        for(int i=0; i<numSeg; i++) {
            meteorIndices[i*3] = topCorner;
            meteorIndices[i*3+1] = i;
            meteorIndices[i*3+2] = (i+1)%numSeg;
            meteorIndices[3*numSeg+i*3] = bottomCorner;
            meteorIndices[3*numSeg+i*3+1] = (numRing-2)*numSeg+i;
            meteorIndices[3*numSeg+i*3+2] = (numRing-2)*numSeg+((i+1)%numSeg);
        }

        //calculate faces of other rings
        for(int i=0; i<numRing-2; i++) {
            int indexOffset = (i+1)*(6*numSeg);
            for(int j=0; j<numSeg; j++) {
                meteorIndices[indexOffset+j*6+0] = i*numSeg+j;
                meteorIndices[indexOffset+j*6+1] = i*numSeg+(j+1)%numSeg;
                meteorIndices[indexOffset+j*6+2] = (i+1)*numSeg+(j+1)%numSeg;
                meteorIndices[indexOffset+j*6+3] = i*numSeg+j;
                meteorIndices[indexOffset+j*6+4] = (i+1)*numSeg+(j+1)%numSeg;
                meteorIndices[indexOffset+j*6+5] = (i+1)*numSeg+j;
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
