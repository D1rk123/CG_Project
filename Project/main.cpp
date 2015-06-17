#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <list>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <IL/il.h>
#include "Mesh.hpp"
#include "FlappyBird.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "Camera.h"
#include "BoundingEllipsoid.hpp"

using namespace std;
using glm::vec4;
using glm::vec3;
using glm::mat4;
using std::list;

const char* vertexShaderName = "minimal.vert";
const char* fragmentShaderName = "minimal.frag";
ShaderProgram shaderProgram = ShaderProgram();
list<Mesh> meshes;
Mesh sphere = Mesh();
FlappyBird bird = FlappyBird();

Texture texture = Texture(), texture2 = Texture();
mat4 cameraMat = mat4();

int lastTime;
int diffTime;

Camera gCamera;
float deltaAngleX = 0.0f;
float deltaAngleY = 0.0f;
int xOrigin = -1;
int yOrigin = -1;

float energyRate = 0.01f;
bool jumped = false;

GLuint cameraMatrixLocation, orientationMatrixLocation, samplerLocation;
BoundingEllipsoid bEllip;
bool drawEllips = true;

int temp=0;

// GLUT callback Handlers
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    gCamera.setPosition(glm::vec3(0.0f,0.0f,100.0f));
    gCamera.setViewportAspectRatio(ar);

    glViewport(0, 0, width, height);
}


void updateCamera(string direction)
{
    //move position of camera based on WASD keys
    const float moveSpeed = 1.0; //units per second

    if(direction.compare("zoomin")==0){
        gCamera.offsetPosition(diffTime * moveSpeed * -gCamera.forward());
    } else if(direction.compare("zoomout")==0){
        gCamera.offsetPosition(diffTime * moveSpeed * gCamera.forward());
    } else if(direction.compare("left")==0){
        gCamera.offsetPosition(diffTime * moveSpeed * -gCamera.right());
    } else if(direction.compare("right")==0){
        gCamera.offsetPosition(diffTime * moveSpeed * gCamera.right());
    } else if(direction.compare("up")==0){
        gCamera.offsetPosition(diffTime * moveSpeed * -glm::vec3(0,1,0));
    } else if(direction.compare("down")==0){
        gCamera.offsetPosition(diffTime * moveSpeed * glm::vec3(0,1,0));
    }
}

float getTimeFactorBetweenUpdates() {
    //cout << diffTime << endl;
    float timeseconds = (float)diffTime/1000;
    float factor = 1.0f;
    return timeseconds*factor;
}

/*void jump(){
    //bird.changeEnergy(-2*energyRate);
    bird.increaseJumpVelocity();
}*/

void updateBirdMovement() {
    bird.changeEnergy(energyRate);

    // update flymovement
    bird.increaseFallVelocity(getTimeFactorBetweenUpdates());
    glm::vec3 update = bird.getMesh()->getMovement()*getTimeFactorBetweenUpdates();

    bird.getMesh()->transform(glm::translate(update));

    if (temp < 100) {
        cout << glm::to_string(update/getTimeFactorBetweenUpdates()) << endl;
        temp++;
    }
    // make camera move with bird
    gCamera.offsetPosition(bird.getFlyVelocity()*getTimeFactorBetweenUpdates() * gCamera.right());
}


static void display(void)
{
    diffTime = glutGet(GLUT_ELAPSED_TIME) - lastTime;
    lastTime = glutGet(GLUT_ELAPSED_TIME);

    //Clear the render buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Select which shader program we use
    glUseProgram(shaderProgram.getName());

    // send the camera matrix to the shader
    glUniformMatrix4fv(cameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(gCamera.matrix()));

    // Update gravity for bird
    updateBirdMovement();

    // Find rotation matrix to make flappybird face in direction of movement
    glm::vec3 dir = glm::normalize(bird.getMesh()->getMovement());
    glm::vec3 z = glm::vec3(0.0f,0.0f,1.0f);
    glm::vec3 cross = glm::cross(dir,z);
    glm::mat4 flightRotation = glm::mat4(glm::vec4(dir, 0.0f), glm::vec4(z,0.0f), glm::vec4(cross, 0.0f), glm::vec4(0,0,0,1.0f));

   //if(!meshes.back().testCollision(meshes.front())) {
        //move the model

    //    meshes.front().transform(glm::translate(vec3(0.01f, 0.0f, 0.0f)));
    //    meshes.back().transform(glm::rotate(0.01f, vec3(0.0f, 1.0f, 0.0f)));
    //}

    //send the texture selection to the shader
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(samplerLocation, 0/*GL_TEXTURE0*/);

    for(std::list<Mesh>::iterator iter = meshes.begin(); iter != meshes.end(); iter++) {
        glBindTexture(GL_TEXTURE_2D, texture.getName());
        //send the orientation matrix to the shader

        glm::mat4 sendOrientation = iter->getOrientation()*flightRotation;
        glUniformMatrix4fv(orientationMatrixLocation, 1, GL_FALSE, glm::value_ptr(sendOrientation));

        iter->draw();
        if(drawEllips) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

            glBindTexture(GL_TEXTURE_2D, texture2.getName());

            mat4 ellipOrientation = iter->getOrientation() * iter->getEllip().orientation * flightRotation;
            //send the orientation matrix to the shader
            glUniformMatrix4fv(orientationMatrixLocation, 1, GL_FALSE, glm::value_ptr(ellipOrientation));
            sphere.draw();

            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
    }

    //swap the renderbuffer with the screenbuffer
    glutSwapBuffers();
}

static void mouseClick(int button, int state, int x, int y)
{
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			xOrigin = -1;
			yOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
			yOrigin = y;
		}
	}
}

static void mouseMove(int x, int y)
{
	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngleX = (x - xOrigin) * 0.0025f;
		deltaAngleY = (y - yOrigin) * 0.0025f;

		// update camera's direction
        gCamera.offsetOrientation(-deltaAngleY, -deltaAngleX);

	}
}

static void keyUp(unsigned char key, int x, int y) {
    if (key == ' ') {
        jumped = false;
    }
}

static void key(unsigned char key, int x, int y)
{
    //empty if statement to remove unused argument warning

    if(x == y)
    {
    }
    switch (key)
    {
        case 'q':
            exit(0);
            break;
        case 's':
            updateCamera("zoomin");
            break;
        case 'w':
            updateCamera("zoomout");
            break;
        case 'a':
            updateCamera("left");
            break;
        case 'd':
            updateCamera("right");
            break;
        case 'x':
            updateCamera("down");
            break;
        case 'z':
            updateCamera("up");
            break;
        case 'l':
            drawEllips = !drawEllips;
            break;
        case ' ':
            if (jumped == false) {
                bird.increaseJumpVelocity();
            }
            jumped = true;
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
    //Initialize GLUT and create window in which we can draw using OpenGL
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
    cout << "Turn drawing collision ellipsoids on/off with the l key." << endl;

    if (!glewIsSupported("GL_VERSION_3_3"))
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
    glutKeyboardUpFunc(keyUp);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);
    glutIdleFunc(idle);

    glClearColor(0,0,0,1);

    //Enable backface culling causing clockwise oriented triangles to not be rendered
    glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //uncomment to draw wireframes
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //Set up shaders
    shaderProgram.setupShaders(vertexShaderName, fragmentShaderName);
    //Set uniform variable locations
    cameraMatrixLocation = glGetUniformLocation(shaderProgram.getName(), "camera");
    assert(cameraMatrixLocation != 0xFFFFFFFF);
    samplerLocation = glGetUniformLocation(shaderProgram.getName(), "textureSampler");
    assert(samplerLocation != 0xFFFFFFFF);
    orientationMatrixLocation = glGetUniformLocation(shaderProgram.getName(), "orientation");
    assert(orientationMatrixLocation != 0xFFFFFFFF);

    //seed the random number generator
    srand(time(0));

    Geometry geom1, geom2, geomSphere;
    geom1.loadOBJ("models/FlappyBirdFinished.obj", true);
    //geom1.loadOBJ("models/testUnit.obj", true);
    //geom1.loadOBJ("models/Satellite1.obj", false);

    geom2.makeRandomMeteor(15,15,12,0.04f);
    //geom1.makeRandomMeteor(3, 3, 0, 0.08f);
    geomSphere.makeSphere(20, 20);

    //Make a mesh
    sphere.makeMesh(geomSphere);

    meshes.push_back(Mesh(geom1));
    meshes.push_back(Mesh(geom2));
    bird = FlappyBird(&meshes.front());
    bird.startFlying();

    //clear geometry memory, because it had been copied to the video card
    geom1.remove();
    geom2.remove();
    geomSphere.remove();

    //Load a texture
    texture.load("textures/meteor.jpg");
    //Load a texture
    texture2.load("textures/white.png");

    lastTime = glutGet(GLUT_ELAPSED_TIME);
    //Start the GLUT loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
