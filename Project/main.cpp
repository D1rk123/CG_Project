#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include "Mesh.hpp"
#include "ShaderHandler.hpp"

using namespace std;
using glm::vec4;
using glm::vec3;

const char* vertexShaderName = "minimal.vert";
const char* fragmentShaderName = "minimal.frag";
ShaderHandler shaderHandler = ShaderHandler();
Mesh mesh = Mesh();

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);

    glLoadIdentity();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderHandler.getShaderProgram());

    //glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrixPipeline.getResult().getArray());
    //glUniform3fv(cameraToObjectLocation, 1, cameraToObjectDirection.getArray());

    //glUniform1i(modelSampler, 0);
    //glUniform1i(cubeShadeSampler, 1);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.getVBO());
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)sizeof(Vector3d<float>));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIBO());
    glDrawElements(GL_TRIANGLES, mesh.getIndicesCount(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);

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

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        cout << "Error: " << glewGetErrorString(res);
        return 1;
    }

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0,0,0,1);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    mesh.makeBuffers();
    shaderHandler.setupShaders(vertexShaderName, fragmentShaderName);

    glutMainLoop();

    return EXIT_SUCCESS;
}
