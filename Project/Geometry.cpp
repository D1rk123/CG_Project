#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/random.hpp>
#include "Geometry.hpp"

const double pi = 3.141592653589793238462643383279502884;
const double epsilon = 0.0001;

using glm::vec3;
using glm::vec2;
using std::vector;
using std::cout;
using std::endl;

bool Geometry::loadOBJ(const char * path, bool hasTexture)
{
    vector < unsigned int > vertexIndices, uvIndices, normalIndices;
    vector < vec3 > temp_vertices;
    vector < vec2 > temp_uvs;
    vector < vec3 > temp_normals;

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
            vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        } else if(strcmp(lineHeader, "vt") == 0) {
            vec2 vertexUV;
            fscanf(file, "%f %f\n", &vertexUV.x, &vertexUV.y);
            temp_uvs.push_back(vertexUV);
        } else if(strcmp(lineHeader, "vn") == 0) {
            vec3 vertexNormal;
            fscanf(file, "%f %f %f\n", &vertexNormal.x, &vertexNormal.y, &vertexNormal.z);
            temp_normals.push_back(vertexNormal);
        } else if(strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            if(hasTexture) {
                int amount = fscanf(file, " %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if(amount != 9) {
                    printf("Obj file is invalid for this parser.");
                    return false;
                }
                for( unsigned int i = 0; i < 3; i++) {
                    vertexIndices.push_back(vertexIndex[i]);
                    uvIndices.push_back(uvIndex[i]);
                    normalIndices.push_back(normalIndex[i]);
                }
            } else {
                int amount = fscanf(file, " %d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
                if(amount != 6) {
                    printf("Obj file is invalid for this parser.");
                    return false;
                }
                for( unsigned int i = 0; i < 3; i++) {
                    vertexIndices.push_back(vertexIndex[i]);
                    normalIndices.push_back(normalIndex[i]);
                }
            }
        }
        //else {
        //    printf("Found something else than v, vt, vn or f. Problem?");
        //}
    }

    numIndices = vertexIndices.size();

    vertices = new Vertex[numIndices];
    numVertices = numIndices;
    indices = new unsigned int[numVertices];

    for(GLsizei i = 0; i < numIndices; i++) {
        if(hasTexture) {
            vertices[i] = Vertex(temp_vertices[vertexIndices[i]-1], temp_normals[normalIndices[i]-1], temp_uvs[uvIndices[i]-1]);
        } else {
            vertices[i] = Vertex(temp_vertices[vertexIndices[i]-1], temp_normals[normalIndices[i]-1], glm::vec2(0.0, 0.0));
        }
        indices[i] = i;
    }
    return true;
}

glm::vec3 Geometry::calcTranformedPos(vec3 pos, vec3* directions, int numDirections, float noiseLength) {
    float length = 0.5;
    for(int k=0; k<numDirections; k++) {
        float dot = std::max(glm::dot(pos, directions[k]), 0.0f);
        length += dot*dot*dot*0.5;
    }
    return pos*length+glm::ballRand(noiseLength);
}

glm::vec3 Geometry::calcTriangleNormal(GLuint* indices) {
    vec3 edge1 = vertices[indices[0]].pos   - vertices[indices[1]].pos;
    vec3 edge2 = vertices[indices[2]].pos - vertices[indices[1]].pos;

    return glm::normalize(glm::cross(edge2, edge1));
}

void Geometry::makeRandomMeteor(int numSeg, int numRing, int numDirections, float noiseLength)
{
    assert(numSeg >= 3);
    assert(numRing >= 2);

    vec3 directions[numDirections];

    //generate random vectors to extrude in that direction
    for(int i=0; i<numDirections; i++) {
        directions[i] = glm::ballRand(1.0f);
    }

    numVertices = (numRing-1)*(numSeg+1)+2;
    numIndices = 3*(numRing-1)*numSeg*2;
    vertices = new Vertex[numVertices];
    indices = new unsigned int[numIndices];

    //Calculate vertex positions
    for(int i=1; i<numRing; i++) {
        double theta = (double(i)/numRing)*pi;
        double sinTheta = sin(theta);
        double cosTheta = cos(theta);
        for(int j=0; j<(numSeg+1); j++) {
            double phi = (double(j)/numSeg)*2*pi;
            double sinPhi = sin(phi);
            double cosPhi = cos(phi);
            vec3 position = vec3(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
            vertices[(i-1)*(numSeg+1)+j] = Vertex(calcTranformedPos(position, directions, numDirections, noiseLength), position, vec2(double(j)/numSeg, double(i)/numRing));
        }
        vertices[(i-1)*(numSeg+1)+numSeg].pos = vertices[(i-1)*(numSeg+1)].pos;
    }

    //Calculate upper and lower ring faces
    int topCorner = (numRing-1)*(numSeg+1);
    int bottomCorner = topCorner+1;

    vertices[topCorner] =    Vertex(calcTranformedPos(vec3(0,0,1), directions, numDirections, noiseLength),
                                    vec3(0,0,1), vec2(0.5f,1.0f));
    vertices[bottomCorner] = Vertex(calcTranformedPos(vec3(0,0,-1), directions, numDirections, noiseLength),
                                    vec3(0,0,-1), vec2(0.5f,0.0f));

    int numTriangles = numIndices/3;
    vector<vec3> triangleNormals = vector<vec3>();
    triangleNormals.reserve(numTriangles);
    vector<vector<int> > connected = vector<vector<int> >(numVertices);

    for(int i=0; i<numSeg; i++) {
        //Make top ring triangles
        indices[i*3] = topCorner;
        indices[i*3+1] = i;
        indices[i*3+2] = i+1;
        connected[topCorner].push_back(i);
        connected[i        ].push_back(i);
        connected[i+1      ].push_back(i);
        //Make bottom ring triangles
        indices[3*numSeg+i*3] = bottomCorner;
        indices[3*numSeg+i*3+1] = (numRing-2)*(numSeg+1)+i+1;
        indices[3*numSeg+i*3+2] = (numRing-2)*(numSeg+1)+i;
        connected[bottomCorner].push_back(numSeg+i);
        connected[(numRing-2)*(numSeg+1)+i+1].push_back(numSeg+i);
        connected[(numRing-2)*(numSeg+1)+i  ].push_back(numSeg+i);
    }
    //store the connections of the right most vertices to the left most triangles and vice versa
    connected[0].push_back(numSeg-1);
    connected[numSeg-1].push_back(0);
    connected[numSeg].push_back(2*numSeg-1);
    connected[2*numSeg-1].push_back(numSeg);

    //calculate faces of other rings
    for(int i=0; i<numRing-2; i++) {
        int indexOffset = (i+1)*(6*((numSeg+1)-1));
        for(int j=0; j<numSeg; j++) {
            indices[indexOffset+j*6+0] = i*(numSeg+1)+j;
            indices[indexOffset+j*6+1] = (i+1)*(numSeg+1)+j+1;
            indices[indexOffset+j*6+2] = i*(numSeg+1)+j+1;
            connected[i*(numSeg+1)+j      ].push_back((indexOffset+j*6)/3);
            connected[(i+1)*(numSeg+1)+j+1].push_back((indexOffset+j*6)/3);
            connected[i*(numSeg+1)+j+1    ].push_back((indexOffset+j*6)/3);

            indices[indexOffset+j*6+3] = i*(numSeg+1)+j;
            indices[indexOffset+j*6+4] = (i+1)*(numSeg+1)+j;
            indices[indexOffset+j*6+5] = (i+1)*(numSeg+1)+j+1;
            connected[i*(numSeg+1)+j      ].push_back((indexOffset+j*6)/3+1);
            connected[(i+1)*(numSeg+1)+j  ].push_back((indexOffset+j*6)/3+1);
            connected[(i+1)*(numSeg+1)+j+1].push_back((indexOffset+j*6)/3+1);
        }
        //store the connections of the right most vertices to the left most triangles and vice versa
        connected[i*(numSeg+1)].push_back((indexOffset+(numSeg-1)*6)/3);
        connected[(i+1)*(numSeg+1)].push_back((indexOffset+(numSeg-1)*6)/3);
        connected[i*(numSeg+1)+(numSeg-1)+1].push_back(indexOffset/3);
        connected[(i+1)*(numSeg+1)+(numSeg-1)+1].push_back(indexOffset/3);

    }

    for(int i=0; i<numTriangles; i++) {
        triangleNormals[i] = calcTriangleNormal(&indices[i*3]);
    }

    //calculate the normal for each vertex by combining the normals of the connected faces
    for(int i=0; i<numVertices; i++) {
        vec3 normal = vec3(0);
        for(size_t j=0; j<connected[i].size(); j++) {
            normal += triangleNormals[connected[i][j]];
        }
        vertices[i].normal = glm::normalize(normal);
        //cout << glm::to_string(vertices[i].normal) << endl;
    }

    /*for(int i=0; i<numVertices; i++) {
        std::cout << i << ": " << vertices[i] << std::endl;
    }
    for(int i=0; i<numIndices/3; i++) {
        std::cout << i << ": " << indices[i*3] << "/" << indices[i*3+1] << "/" << indices[i*3+2] << std::endl;
    }*/
}

/*void Geometry::calculateNormals() {
    int numTriangles = numIndices/3;
    vector<vec3> triangleNormals = vector<vec3>(numTriangles);
    vector<vector<int> > connected = vector<vector<int> >(numVertices);

    //calculate the normal for each triangle
    //and add a reference to the normal to each vertex of the same triangle
    for(int i=0; i<numTriangles; i++) {
        triangleNormals[i] = calcTriangleNormal(&indices[i*3]);
        connected[indices[i*3]].push_back(i);
        connected[indices[i*3+1]].push_back(i);
        connected[indices[i*3+2]].push_back(i);
    }

    //calculate the normal for each vertex by combining the normals of the connected faces
    for(int i=0; i<numVertices; i++) {
        vec3 normal = vec3(0);
        for(size_t j=0; j<connected[i].size(); j++) {
            normal += triangleNormals[connected[i][j]];
        }
        vertices[i].normal = glm::normalize(normal);
        //cout << glm::to_string(vertices[i].normal) << endl;
    }
}*/
