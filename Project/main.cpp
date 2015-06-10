#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <IL/il.h>
#include "Mesh.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"

using namespace std;
using glm::vec4;
using glm::vec3;
using glm::mat4;

const char* vertexShaderName = "minimal.vert";
const char* fragmentShaderName = "minimal.frag";
ShaderProgram shaderProgram = ShaderProgram();
Mesh mesh = Mesh();
Texture texture = Texture();
mat4 cameraMat = mat4();
GLuint matrixLocation, samplerLocation;

// GLUT callback Handlers
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    cameraMat = glm::perspective(75.0f, ar, 0.1f, 100.f);
    cameraMat = glm::translate(cameraMat, vec3(0.0f, -0.5f, -5.0f));

    glViewport(0, 0, width, height);
}

static void display(void)
{
    //Clear the render buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Select which shader program we use
    glUseProgram(shaderProgram.getName());

    //rotate the model
    cameraMat = glm::rotate(cameraMat, 0.1f, vec3(0,1,0));

    //send the camera matrix to the shader
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(cameraMat));
    //send the texture selection to the shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.getName());
    glUniform1i(samplerLocation, 0/*GL_TEXTURE0*/);

    //Select the mesh vertices
    glBindBuffer(GL_ARRAY_BUFFER, mesh.getVBO());
    //select the vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    //select the normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec3));
    //select the texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec3)*2));

    //select the elements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIBO());
    //draw them as triangles
    glDrawElements(GL_TRIANGLES, mesh.getIndicesCount(), GL_UNSIGNED_INT, 0);

    //disable the vertex attribute arrays, because the next shader we use might not use them
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    //swap the renderbuffer with the screenbuffer
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

//Program entry point
int main(int argc, char *argv[])
{
    //Initialize GLUT and create window in wich we can draw using OpenGL
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Flappy Bird... With a gun... In Space...");

    //Initialize GLEW, this should be done after an OpenGL window is created
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        cout << "Error: " << glewGetErrorString(res);
        return 1;
    }

    const char* version = (const char*)glGetString(GL_VERSION);
    cout << "OpenGL Version:" << version << endl;

    if (!glewIsSupported("GL_VERSION_4_2"))
    {
        cout << "GLEW version out of date: please update your videocard driver" << endl;
        return 1;
    }

    //Check if the version of DevIL in the .dll-file is the same or a newer version then the one the engine is compiled with.
    //If the version is older give an error message and stop the function.
    if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
    {
        cout << "DevIL version out of date: please update the dll file." << endl;
        return 1;
    }
    ilInit();

    //Set GLUT callback functions
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0,0,0,1);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //Make a cube Mesh
    mesh.makeCube(vec3(0.0, 0.0, 0.0), vec3(0.3, 0.6, 0.5));
    //Load a texture
    texture.load("FlappyBirdTexture.png");
    //Set up shaders
    shaderProgram.setupShaders(vertexShaderName, fragmentShaderName);
    //Set uniform variable locations
    matrixLocation = glGetUniformLocation(shaderProgram.getName(), "camera");
    assert(matrixLocation != 0xFFFFFFFF);
    samplerLocation = glGetUniformLocation(shaderProgram.getName(), "textureSampler");
    assert(samplerLocation != 0xFFFFFFFF);

    //Start the GLUT loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
