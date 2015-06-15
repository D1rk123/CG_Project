#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>
#include "Mesh.hpp"
#include "glmToCout.hpp"

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::mat4;
using glm::mat3;
using std::cout;
using std::endl;

void Mesh::draw() {
    //Select the mesh vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //select the vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    //select the normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
    //select the texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)*2));

    //select the elements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //draw them as triangles
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

    //disable the vertex attribute arrays, because the next shader we use might not use them
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Mesh::makeMesh(const Geometry& geom) {
    indicesCount = geom.getNumIndices();
    //Generate a VBO and store vertex data in it
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*geom.getNumVertices(), geom.getVertices(), GL_STATIC_DRAW);
    //Generate a IBO and store triangle index data in it
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*geom.getNumIndices(), geom.getIndices(), GL_STATIC_DRAW);

    ellip = geom.approxBoundingEllipsoid();
}

bool Mesh::testCollision(const Mesh& other) {
    //Bounding Sphere test
    vec4 otherPosition = other.orientation[3] + other.ellip.orientation[3];
    vec4 diff4 = orientation[3] + ellip.orientation[3] - otherPosition;
    vec3 diff = vec3(diff4[0], diff4[1], diff4[2]);
    float squaredDiff = glm::dot(diff, diff);
    float minDiff = ellip.radius + other.ellip.radius;
    minDiff *= minDiff;
    if (squaredDiff > minDiff) {
        return false;
    }

    //if the Bounding Sphere test is positive continue with the bounding ellipsoid test
    //test based on http://courses.cs.washington.edu/courses/csep521/07wi/prj/duzak.pdf
    //but fixed a mistake in that document, because with the original formula each point gets further away from the center of ellipsoid 1
    //this was fixed by adding an additional translation back to the center (which is the direction of the second derivative of the sphere).

    otherPosition[3] = 1.0f;
    mat4 myOrientation = orientation * ellip.orientation;
    mat4 myInvOrientation = glm::inverse(myOrientation);
    vec4 otherPositionInMyspace = myInvOrientation * otherPosition;
    //take the starting point in the direction of the other ellipsoid
    vec3 point = glm::normalize(vec3(otherPositionInMyspace[0], otherPositionInMyspace[1], otherPositionInMyspace[2]));

    mat4 meToOther = glm::inverse(other.orientation * other.ellip.orientation) * myOrientation;
    mat3 meToOther3 = mat3(meToOther[0][0], meToOther[0][1], meToOther[0][2],
                           meToOther[1][0], meToOther[1][1], meToOther[1][2],
                           meToOther[2][0], meToOther[2][1], meToOther[2][2]);

    vec3 randomvector1 = vec3(1.0f, 0.0f, 0.0f), randomvector2 = vec3(0.0f, 1.0f, 0.0f);
    vec3 aVec;
    if(glm::dot(point, randomvector1) < glm::dot(point, randomvector2)) {
        aVec = glm::cross(point, randomvector1);
    } else {
        aVec = glm::cross(point, randomvector2);
    }
    float lastDistance = minDiff * 2;
    //int i=0;
    while(true) {
        //i++;
        vec3 tangent = glm::normalize(glm::cross(point, aVec));
        vec3 biTangent = glm::cross(point, tangent);
        vec3 toCenter = -point;

        vec4 transfPoint4 = meToOther * vec4(point, 1.0f);
        vec3 transfPoint = vec3(transfPoint4[0], transfPoint4[1], transfPoint4[2]);
        float distance = glm::l2Norm(transfPoint);

        if (distance<1.001f) {
            //cout << i<< endl;
            return true;
        }
        if ((lastDistance - distance)<0.001f) {
            //cout << i<< endl;
            return false;
        }
        lastDistance = distance;

        vec3 normal = transfPoint / distance;
        vec3 transfTangent = meToOther3 * tangent;
        vec3 transfBiTangent = meToOther3 * biTangent;
        vec3 transfToCenter = meToOther3 * toCenter;

        float projTangent = glm::dot(normal, transfTangent);
        float projBiTangent = glm::dot(normal, transfBiTangent);
        float projToCenter = glm::dot(normal, transfToCenter);

        float movTangent = projTangent / projToCenter;
        float movBiTangent = projBiTangent / projToCenter;
        float movToCenter = 1-sqrt(1 - (movTangent*movTangent + movBiTangent*movBiTangent));

        point -= tangent * movTangent + biTangent * movBiTangent - movToCenter * toCenter;
    }
}
