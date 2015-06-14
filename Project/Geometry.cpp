#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/random.hpp>
#include "Geometry.hpp"

const double pi = 3.141592653589793238462643383279502884;
const double epsilon = 0.0001;

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::mat4;
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

glm::vec3 Geometry::calcTriangleNormal(GLuint* indices) {
    vec3 edge1 = vertices[indices[0]].pos   - vertices[indices[1]].pos;
    vec3 edge2 = vertices[indices[2]].pos - vertices[indices[1]].pos;

    return glm::normalize(glm::cross(edge2, edge1));
}

void Geometry::addTriangleAndConnections(GLuint triangle, GLuint vert1, GLuint vert2, GLuint vert3) {
    indices[triangle*3] = vert1;
    indices[triangle*3+1] = vert2;
    indices[triangle*3+2] = vert3;
    vertToTriConnections[vert1].push_back(triangle);
    vertToTriConnections[vert2].push_back(triangle);
    vertToTriConnections[vert3].push_back(triangle);
}

void Geometry::makeSphere(int numSeg, int numRing)
{
    assert(numSeg >= 3);
    assert(numRing >= 2);

    numVertices = (numRing-1)*(numSeg+1)+2;
    numIndices = 3*(numRing-1)*numSeg*2;
    vertices = new Vertex[numVertices];
    indices = new unsigned int[numIndices];
    vertToTriConnections = vector<vector<int> >(numVertices);

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
            vertices[(i-1)*(numSeg+1)+j] = Vertex(position, position, vec2(double(j)/numSeg, double(i)/numRing));
        }
    }

    //Calculate upper and lower ring faces
    int topCorner = (numRing-1)*(numSeg+1);
    int bottomCorner = topCorner+1;

    vertices[topCorner] =    Vertex(vec3(0,0,1), vec3(0,0,1), vec2(0.5f,0.0f));
    vertices[bottomCorner] = Vertex(vec3(0,0,-1), vec3(0,0,-1), vec2(0.5f,1.0f));

    for(int i=0; i<numSeg; i++) {
        //Make top ring triangles
        addTriangleAndConnections(i, topCorner, i, i+1);
        //Make bottom ring triangles
        addTriangleAndConnections(numSeg+i, bottomCorner, (numRing-2)*(numSeg+1)+i+1, (numRing-2)*(numSeg+1)+i);
    }
    //store the connections of the right most vertices to the left most triangles and vice versa
    vertToTriConnections[0].push_back(numSeg-1);
    vertToTriConnections[numSeg-1].push_back(0);
    vertToTriConnections[numSeg].push_back(2*numSeg-1);
    vertToTriConnections[2*numSeg-1].push_back(numSeg);

    //calculate faces of other rings
    for(int i=0; i<numRing-2; i++) {
        int indexOffset = (i+1)*(2*((numSeg+1)-1));
        for(int j=0; j<numSeg; j++) {
            addTriangleAndConnections(indexOffset+2*j, i*(numSeg+1)+j, (i+1)*(numSeg+1)+j+1, i*(numSeg+1)+j+1);
            addTriangleAndConnections(indexOffset+2*j+1, i*(numSeg+1)+j, (i+1)*(numSeg+1)+j, (i+1)*(numSeg+1)+j+1);
        }
        //store the connections of the right most vertices to the left most triangles and vice versa
        vertToTriConnections[i*(numSeg+1)].push_back(indexOffset+(numSeg-1)*2);
        vertToTriConnections[(i+1)*(numSeg+1)].push_back(indexOffset+(numSeg-1)*2);
        vertToTriConnections[i*(numSeg+1)+(numSeg-1)+1].push_back(indexOffset);
        vertToTriConnections[(i+1)*(numSeg+1)+(numSeg-1)+1].push_back(indexOffset);

    }

    /*for(int i=0; i<numVertices; i++) {
        std::cout << i << ": " << vertices[i] << std::endl;
    }
    for(int i=0; i<numIndices/3; i++) {
        std::cout << i << ": " << indices[i*3] << "/" << indices[i*3+1] << "/" << indices[i*3+2] << std::endl;
    }*/
}

glm::vec3 Geometry::calcTranformedPos(vec3 pos, vec3* directions, int numDirections, float noiseLength) {
    float length = 0.3;
    for(int k=0; k<numDirections; k++) {
        float dot = std::max(glm::dot(pos, directions[k]), 0.0f);
        length += dot*dot*dot*0.5;
    }
    return pos*length+glm::ballRand(noiseLength);
}

void Geometry::makeRandomMeteor(int numSeg, int numRing, int numDirections, float noiseLength) {
    //Start off with a sphere
    makeSphere(numSeg, numRing);

    //generate random vectors to extrude in those directions
    vec3 directions[numDirections];
    for(int i=0; i<numDirections; i++) {
        directions[i] = glm::ballRand(1.0f);
    }

    //transform all vertices of the sphere
    for(int i=0; i<numVertices; i++) {
        vertices[i].pos = calcTranformedPos(vertices[i].pos, directions, numDirections, noiseLength);
    }

    //Attach the vertices of the seam by giving them the same position
    for(int i=1; i<numRing; i++) {
        vertices[(i-1)*(numSeg+1)+numSeg].pos = vertices[(i-1)*(numSeg+1)].pos;
    }

    calculateNormals();
}

void Geometry::calculateConnections() {
    int numTriangles = numIndices/3;
    vertToTriConnections = vector<vector<int> >(numVertices);
    for(int i=0; i<numTriangles; i++) {
        vertToTriConnections[indices[i*3]].push_back(i);
        vertToTriConnections[indices[i*3+1]].push_back(i);
        vertToTriConnections[indices[i*3+2]].push_back(i);
    }
}

void Geometry::calculateNormals() {
    int numTriangles = numIndices/3;
    vector<vec3> triangleNormals = vector<vec3>(numTriangles);

    //If not yet calculated, calculate which vertices connect to which triangles
    if(vertToTriConnections.size() == 0) {
        calculateConnections();
    }

    //calculate the normal for each triangle
    //and add a reference to the normal to each vertex of the same triangle
    for(int i=0; i<numTriangles; i++) {
        triangleNormals[i] = calcTriangleNormal(&indices[i*3]);
    }

    //calculate the normal for each vertex by combining the normals of the connected faces
    for(int i=0; i<numVertices; i++) {
        vec3 normal = vec3(0);
        for(size_t j=0; j<vertToTriConnections[i].size(); j++) {
            normal += triangleNormals[vertToTriConnections[i][j]];
        }
        vertices[i].normal = glm::normalize(normal);
        //cout << glm::to_string(vertices[i].normal) << endl;
    }
}

/*BoundingSphere Geometry::approxBoundingSphere() {
    float squaredDiameter = 0.0f;
    vec3 position;
    for(int i=0; i<numVertices-1; i++) {
        for(int j=i+1; j<numVertices; j++) {
            vec3 distance = vertices[i].pos - vertices[j].pos;
            float possibleDiameter = glm::dot(distance, distance);
            if(possibleDiameter > squaredDiameter) {
                squaredDiameter = possibleDiameter;
                position = vertices[j].pos + distance/2.0f;
            }
        }
    }
    return BoundingSphere(position, sqrt(squaredDiameter)/2.0f);
}*/


//TODO: clean up code
BoundingEllipsoid Geometry::approxBoundingEllipsoid() const {

    float squaredDiameter = 0.0f;
    vec3 position;
    vec3 direction[3];
    vec3 normalizedDirection[3];
    float radius[3];

    for(int i=0; i<numVertices-1; i++) {
        for(int j=i+1; j<numVertices; j++) {
            vec3 distance = vertices[i].pos - vertices[j].pos;
            float possibleDiameter = glm::dot(distance, distance);
            if(possibleDiameter > squaredDiameter) {
                squaredDiameter = possibleDiameter;
                direction[0] = distance/2.0f;

                position = vertices[j].pos + direction[0];
            }
        }
    }
    radius[0] = sqrtf(squaredDiameter)/2.0f;
    normalizedDirection[0] = direction[0]/radius[0];
    squaredDiameter = 0.0f;
    float offset;

    for(int i=0; i<numVertices-1; i++) {
        for(int j=i+1; j<numVertices; j++) {
            vec3 distance = vertices[i].pos - vertices[j].pos;
            vec3 projectedDistance = distance - normalizedDirection[0] * glm::dot(distance, normalizedDirection[0]);
            float possibleDiameter = glm::dot(projectedDistance, projectedDistance);
            if(possibleDiameter > squaredDiameter) {
                squaredDiameter = possibleDiameter;
                direction[1] = projectedDistance/2.0f;

                radius[1] = sqrtf(squaredDiameter)/2.0f;
                normalizedDirection[1] = direction[1]/radius[1];
                offset = glm::dot(vertices[j].pos, normalizedDirection[1]) - glm::dot(position, normalizedDirection[1]) + radius[1];
            }
        }
    }

    position += normalizedDirection[1] * offset;

    normalizedDirection[2] = glm::cross(normalizedDirection[0], normalizedDirection[1]);
    squaredDiameter = 0.0f;

    for(int i=0; i<numVertices-1; i++) {
        for(int j=i+1; j<numVertices; j++) {
            vec3 distance = vertices[i].pos - vertices[j].pos;
            vec3 projectedDistance = normalizedDirection[2] * glm::dot(distance, normalizedDirection[2]);
            float possibleDiameter = glm::dot(projectedDistance, projectedDistance);
            if(possibleDiameter > squaredDiameter) {
                squaredDiameter = possibleDiameter;
                direction[2] = projectedDistance/2.0f;

                radius[2] = sqrtf(squaredDiameter)/2.0f;
                normalizedDirection[2] = direction[2]/radius[2];
                offset = glm::dot(vertices[j].pos, normalizedDirection[2]) - glm::dot(position, normalizedDirection[2]) + radius[2];
            }
        }
    }

    position += normalizedDirection[2] * offset;

    /*cout << "Should be 0: " << glm::dot(direction[0], direction[1]) << endl;
    cout << "Should be 0: " << glm::dot(direction[1], direction[2]) << endl;
    cout << "Should be 0: " << glm::dot(direction[2], direction[0]) << endl;

    cout << glm::to_string(direction[0]) << "length: " << glm::l2Norm(direction[0]) << endl;
    cout << glm::to_string(direction[1]) << "length: " << glm::l2Norm(direction[1]) << endl;
    cout << glm::to_string(direction[2]) << "length: " << glm::l2Norm(direction[2]) << endl;
    cout << glm::to_string(position) << "length: " << glm::l2Norm(position) << endl;*/

    mat4 result(vec4(direction[0], 0.0f), vec4(direction[1], 0.0f), vec4(direction[2], 0.0f), vec4(position, 1.0f));
    //cout << glm::to_string(result) << endl;

    return BoundingEllipsoid(result, radius[0]);
}
