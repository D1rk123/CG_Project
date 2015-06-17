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
#include "Lazor.hpp"
#include "FlappyBird.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "Camera.h"
#include "BoundingEllipsoid.hpp"
#include <vector>

using namespace std;
using glm::vec4;
using glm::vec3;
using glm::mat4;
using std::list;

const char* skyboxVertexShaderName = "skybox.vert";
const char* skyboxFragmentShaderName = "flat.frag";
const char* phongVertexShaderName = "phong.vert";
const char* phongFragmentShaderName = "phong.frag";
const char* flatVertexShaderName = "flat.vert";
const char* flatFragmentShaderName = "flat.frag";

ShaderProgram phongShading, skyboxShading, flatShading;

vector<Mesh> meteorMeshes;
Mesh sphereMesh, lazorMesh, skyboxMesh, flappyMesh;

FlappyBird bird;

Texture meteorTexture, sphereTexture, skyboxTexture, flappyTexture;
Texture lazorTextures[3];

int lastTime;
int diffTime;

Camera gCamera;
float deltaAngleX = 0.0f;
float deltaAngleY = 0.0f;
int xOrigin = -1;
int yOrigin = -1;
bool jumped = false;

GLuint phongCameraMatrixLocation, phongOrientationMatrixLocation, phongSamplerLocation;
GLuint skyboxOffsetLocation, skyboxSamplerLocation;
GLuint flatCameraMatrixLocation, flatOrientationMatrixLocation, flatSamplerLocation;

BoundingEllipsoid bEllip;
bool drawEllips = true;

list<Lazor> lazors;
list<GameObject> meteors;
vector<GameObject*> gameObjects;

// GLUT callback Handlers
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

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

void displaySkyBox() {
    glUseProgram(skyboxShading.getName());

    glDisable(GL_DEPTH_TEST);

    glBindTexture( GL_TEXTURE_2D, skyboxTexture.getName() );

    float offset = gCamera.matrix()[3][0]/50.0f;

    glUniform1f(skyboxOffsetLocation, offset);
    glUniform1i(skyboxSamplerLocation, 0/*GL_TEXTURE0*/);

    skyboxMesh.draw();

    glEnable(GL_DEPTH_TEST);
}

void displayFlappy() {
    // Load texture at frame
    glBindTexture( GL_TEXTURE_2D, flappyTexture.getName() );

    //send the orientation matrix to the shader
    glUniformMatrix4fv(phongOrientationMatrixLocation, 1, GL_FALSE, glm::value_ptr(bird.getOrientation()));

    flappyMesh.draw();
}

void displayMeteors () {
    // Load texture at frame
    glBindTexture( GL_TEXTURE_2D, meteorTexture.getName() );
    for(std::list<GameObject>::iterator iter = meteors.begin(); iter != meteors.end(); iter++) {
        glUniformMatrix4fv(phongOrientationMatrixLocation, 1, GL_FALSE, glm::value_ptr(iter->getOrientation()));
        iter->getMesh().draw();
    }
}
void collectGameObjects () {
    gameObjects.clear();
    gameObjects.push_back(&bird);
    for(std::list<Lazor>::iterator iter = lazors.begin(); iter != lazors.end(); iter++) {
        gameObjects.push_back(&(*iter));
    }
    for(std::list<GameObject>::iterator iter = meteors.begin(); iter != meteors.end(); iter++) {
        gameObjects.push_back(&(*iter));
    }
}
void displayEllipsoids () {
    if(drawEllips) {
        collectGameObjects();
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        glBindTexture(GL_TEXTURE_2D, sphereTexture.getName());
        for(std::vector<GameObject*>::iterator iter = gameObjects.begin(); iter != gameObjects.end(); iter++) {
            mat4 ellipOrientation = (*iter)->getOrientation() * (*iter)->getEllipsoid().orientation;
            //send the orientation matrix to the shader
            glUniformMatrix4fv(flatOrientationMatrixLocation, 1, GL_FALSE, glm::value_ptr(ellipOrientation));
            sphereMesh.draw();
        }
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}

void testCollisions() {
    /*collectGameObjects();
    for(int i=0; i<((int)gameObjects.size())-1; i++) {
        for(size_t j=i+1; j<gameObjects.size(); j++) {
            gameObjects[i]->testCollision(gameObjects[j]);
        }
    }
    if(bird.getCollided()) {
        cout << "GAME OVER!" << endl;
    }
    for(std::list<GameObject>::iterator iter = meteors.begin(); iter != meteors.end(); iter++) {
        if(iter->getCollided()) {
            meteors.erase(iter);
        }
    }
    for(std::list<Lazor>::iterator iter = lazors.begin(); iter != lazors.end(); iter++) {
        if(iter->getCollided()) {
            lazors.erase(iter);
        }
    }*/
}

void updateLazors() {
    for(std::list<Lazor>::iterator iter = lazors.begin(); iter != lazors.end(); iter++) {
        iter->update(getTimeFactorBetweenUpdates());
    }
}

void displayLazors () {
    for(std::list<Lazor>::iterator iter = lazors.begin(); iter != lazors.end(); iter++) {
        glBindTexture( GL_TEXTURE_2D, lazorTextures[iter->getTextureIndex()].getName());
        glUniformMatrix4fv(flatOrientationMatrixLocation, 1, GL_FALSE, glm::value_ptr(iter->getOrientation()));
        lazorMesh.draw();
    }
}


static void display(void)
{
    diffTime = glutGet(GLUT_ELAPSED_TIME) - lastTime;
    lastTime = glutGet(GLUT_ELAPSED_TIME);

    bird.update(getTimeFactorBetweenUpdates());
    updateLazors();

    testCollisions();

    //Clear the render buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);

    // make camera move with bird
    gCamera.offsetPosition(bird.getFlyVelocity()*getTimeFactorBetweenUpdates() * gCamera.right());

    displaySkyBox();

    //Select which shader program we use
    glUseProgram(phongShading.getName());

    // send the camera matrix to the shader
    glUniformMatrix4fv(phongCameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(gCamera.matrix()));

    //send the texture selection to the shader
    glUniform1i(phongSamplerLocation, 0/*GL_TEXTURE0*/);

    displayFlappy();
    displayMeteors();


    glUseProgram(flatShading.getName());
    glUniformMatrix4fv(flatCameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(gCamera.matrix()));

    displayEllipsoids();

    displayLazors();

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
    //empty if statement to remove unused argument warning
    if(x == y)
    {
    }

    if (key == ' ') {
        jumped = false;
    }
}

static void shootLazor()
{
    mat4 orientation = bird.getOrientation();
    vec3 direction = bird.getMovement();

    lazors.push_back(Lazor(orientation, direction, &lazorMesh));
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
        case 'p':
            shootLazor();
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

bool setupOpenGL (int argc, char *argv[]) {
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
        return false;
    }

    const char* version = (const char*)glGetString(GL_VERSION);
    cout << "OpenGL Version:" << version << endl;
    cout << "Turn drawing collision ellipsoids on/off with the l key." << endl;

    if (!glewIsSupported("GL_VERSION_3_3"))
    {
        cout << "GLEW version out of date: please update your videocard driver" << endl;
        return false;
    }

    //Check if the version of DevIL in the .dll-file is the same or a newer version then the one the engine is compiled with.
    //If the version is older give an error message and stop the function.
    if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
    {
        cout << "DevIL version out of date: please update the dll file." << endl;
        return false;
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void setupShaders() {
    phongShading.setupShaders(phongVertexShaderName, phongFragmentShaderName);
    phongCameraMatrixLocation = glGetUniformLocation(phongShading.getName(), "camera");
    assert(phongCameraMatrixLocation != 0xFFFFFFFF);
    phongSamplerLocation = glGetUniformLocation(phongShading.getName(), "textureSampler");
    assert(phongSamplerLocation != 0xFFFFFFFF);
    phongOrientationMatrixLocation = glGetUniformLocation(phongShading.getName(), "orientation");
    assert(phongOrientationMatrixLocation != 0xFFFFFFFF);

    skyboxShading.setupShaders(skyboxVertexShaderName, skyboxFragmentShaderName);
    skyboxOffsetLocation = glGetUniformLocation(skyboxShading.getName(), "offset");
    assert(skyboxOffsetLocation != 0xFFFFFFFF);
    skyboxSamplerLocation = glGetUniformLocation(skyboxShading.getName(), "textureSampler");
    assert(skyboxSamplerLocation != 0xFFFFFFFF);

    flatShading.setupShaders(flatVertexShaderName, flatFragmentShaderName);
    flatSamplerLocation = glGetUniformLocation(flatShading.getName(), "textureSampler");
    assert(flatSamplerLocation != 0xFFFFFFFF);
    flatOrientationMatrixLocation = glGetUniformLocation(flatShading.getName(), "orientation");
    assert(flatOrientationMatrixLocation != 0xFFFFFFFF);
    flatCameraMatrixLocation = glGetUniformLocation(flatShading.getName(), "camera");
    assert(flatCameraMatrixLocation != 0xFFFFFFFF);
}

void setupTextures() {
    meteorTexture.load("textures/meteor.jpg");
    flappyTexture.load("textures/camoFlap.png");
    sphereTexture.load("textures/white.png");
    skyboxTexture.load("textures/skybox.png");

    lazorTextures[0].load("textures/lazor0.png");
    lazorTextures[1].load("textures/lazor1.png");
    lazorTextures[2].load("textures/lazor2.png");
}

void makeMeteorMeshes(int amount) {
    meteorMeshes.reserve(amount);
    for(int i=0; i<amount; i++) {
        Geometry geomMeteor;
        geomMeteor.makeRandomMeteor(15, 15, 12, 0.05f);
        meteorMeshes.push_back(Mesh(geomMeteor));
        geomMeteor.remove();
    }
}

void spawnMeteor(vec3 position) {
    int index = rand()%meteorMeshes.size();

    meteors.push_back(GameObject(&(meteorMeshes[index])));
    meteors.back().transform(glm::translate(position));
}

void setupModels() {
    Geometry geomSphere, geomSkybox, geomFlappy, geomLazor;

    //Make geometry
    //geom1.makeRandomMeteor(15,15,12,0.05f);
    geomSphere.makeSphere(20, 20);
    geomSkybox.makeQuad();
    geomFlappy.loadOBJ("models/testUnit.obj", true);
    geomLazor.loadOBJ("models/lazor.obj", true);

    sphereMesh.makeMesh(geomSphere);
    skyboxMesh.makeMesh(geomSkybox);
    flappyMesh.makeMesh(geomFlappy);
    lazorMesh.makeMesh(geomLazor);

    lazorMesh.setEllipsoid(BoundingEllipsoid(glm::translate(vec3(0.5f, 0.0f, 0.0f))*glm::scale(vec3(1.054378f, 0.211248f, 0.211248f)), 1.054378f));

    bird.setMesh(&flappyMesh);
    bird.startFlying();

    //clear geometry memory, because it had been copied to the video card
    geomSphere.remove();
    geomSkybox.remove();
    geomFlappy.remove();
    geomLazor.remove();

    makeMeteorMeshes(10);

    spawnMeteor(vec3(14.0f, 4.0f, 0.0f));
    spawnMeteor(vec3(14.0f, -4.0f, 0.0f));
    spawnMeteor(vec3(24.0f, 4.0f, 0.0f));
    spawnMeteor(vec3(24.0f, -4.0f, 0.0f));
    spawnMeteor(vec3(34.0f, 4.0f, 0.0f));
    spawnMeteor(vec3(34.0f, -4.0f, 0.0f));
    spawnMeteor(vec3(44.0f, 4.0f, 0.0f));
    spawnMeteor(vec3(44.0f, -4.0f, 0.0f));
    spawnMeteor(vec3(54.0f, 4.0f, 0.0f));
    spawnMeteor(vec3(54.0f, -4.0f, 0.0f));
    spawnMeteor(vec3(64.0f, 4.0f, 0.0f));
    spawnMeteor(vec3(64.0f, -4.0f, 0.0f));
}

//Program entry point
int main(int argc, char *argv[])
{
    setupOpenGL(argc, argv);
    setupShaders();
    setupTextures();
    setupModels();

    gCamera.setPosition(glm::vec3(0.0f,0.0f,24.0f));

    //seed the random number generator
    srand(time(0));

    lastTime = glutGet(GLUT_ELAPSED_TIME);
    //Start the GLUT loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
