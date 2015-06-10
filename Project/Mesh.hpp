#ifndef HPP_MESH
#define HPP_MESH
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <vector>
#include <cstdio>
#include "Vertex.hpp"

using glm::vec3;

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

    bool loadOBJ(
        const char * path
    )
    {
        std::vector < unsigned int > vertexIndices, uvIndices, normalIndices;
        std::vector < glm::vec3 > temp_vertices;
        std::vector < glm::vec2 > temp_uvs;
        std::vector < glm::vec3 > temp_normals;

        std::vector < glm::vec3 > out_vertices;
        std::vector < glm::vec2 > out_uvs;
        std::vector < glm::vec3 > out_normals;

        FILE * file = fopen(path, "r");
        if(file == NULL) {
            printf("File is non-existent.");
            return false;
        }

        while(1) {
            char lineHeader[128];
            int res = fscanf(file, "%s", lineHeader);
            if(res == EOF) {
                break;
            }

            if(strcmp(lineHeader,"v") == 0) {
                glm::vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                temp_vertices.push_back(vertex);
            } else if(strcmp(lineHeader, "vt") == 0) {
                glm::vec2 vertexUV;
                fscanf(file, "%f %f\n", &vertexUV.x, &vertexUV.y);
                temp_uvs.push_back(vertexUV);
            } else if(strcmp(lineHeader, "vn") == 0) {
                glm::vec3 vertexNormal;
                fscanf(file, "%f %f %f\n", &vertexNormal.x, &vertexNormal.y, &vertexNormal.z);
                temp_normals.push_back(vertexNormal);
            } else if(strcmp(lineHeader, "f") == 0) {
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int amount = fscanf(file, " %d//%d %d//%d %d//%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if(amount != 6) {
                    printf("Obj file is invalid for this parser.");
                    return false;
                }
                for( unsigned int i = 0; i < 3; i++) {
                    vertexIndices.push_back(vertexIndex[i]);
                    uvIndices.push_back(uvIndex[i]);
                    normalIndices.push_back(normalIndex[i]);
                }
            }
            //else {
            //    printf("Found something else than v, vt, vn or f. Problem?");
            //}
        }

        for(unsigned int i = 0; i < vertexIndices.size(); i++) {
            unsigned int vertexIndex = vertexIndices[i];
            glm::vec3 vertex = temp_vertices[vertexIndex-1];
            out_vertices.push_back(vertex);
        }

        for(unsigned int i = 0; i < uvIndices.size(); i++) {
            unsigned int vertexUVIndex = uvIndices[i];
            glm::vec2 vertexUV = temp_uvs[vertexUVIndex-1];
            out_uvs.push_back(vertexUV);
        }

        for(unsigned int i = 0; i < normalIndices.size(); i++) {
            unsigned int vertexNormalIndex = normalIndices[i];
            glm::vec3 vertexNormal = temp_normals[vertexNormalIndex-1];
            out_normals.push_back(vertexNormal);
        }

        indicesCount = vertexIndices.size();

        Vertex vertices[indicesCount];
        unsigned int faceIndices[indicesCount];

        for(unsigned int i = 0; i < indicesCount; i++) {
            vertices[i] = Vertex(out_vertices[vertexIndices[i]], out_normals[normalIndices[i]], glm::vec2(0.0, 0.0)); //out_uvs[uvIndices[i]]
            faceIndices[i] = i;
        }

        //Generate a VBO and store vertex data in it
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*indicesCount, vertices, GL_STATIC_DRAW);
        //Generate a IBO and store triangle index data in it
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indicesCount, faceIndices, GL_STATIC_DRAW);

        return true;
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
    void removeBuffers()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }
};

#endif //HPP_MESH
