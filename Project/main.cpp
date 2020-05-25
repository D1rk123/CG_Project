#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <list>
#include <vector>

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
#include "Laser.hpp"
#include "FlappyBird.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "BoundingEllipsoid.hpp"
#include "LaserManager.hpp"
#include "MeteorManager.hpp"


using namespace std;
using glm::vec4;
using glm::vec3;
using glm::mat4;
using std::list;

const char* skyboxVertexShaderName = "shaders\\skybox.vert";
const char* skyboxFragmentShaderName = "shaders\\flat.frag";
const char* phongVertexShaderName = "shaders\\phong.vert";
const char* phongFragmentShaderName = "shaders\\phong.frag";
const char* flatVertexShaderName = "shaders\\flat.vert";
const char* flatFragmentShaderName = "shaders\\flat.frag";

GLuint phongCameraMatrixLocation, phongOrientationMatrixLocation, phongSamplerLocation, phongLazorPositionsLocation;
GLuint skyboxOffsetLocation, skyboxSamplerLocation;
GLuint flatCameraMatrixLocation, flatOrientationMatrixLocation, flatSamplerLocation;

ShaderProgram phongShading, skyboxShading, flatShading;

Mesh sphereMesh, skyboxMesh, flappyMesh;
Texture sphereTexture, skyboxTexture, flappyTexture;

LaserManager laserManager;
MeteorManager meteorManager(100);
FlappyBird bird;
vector<GameObject*> gameObjects;

Camera camera = Camera();

int lastTime, diffTime;

bool jumped = false;

bool drawBoundingEllipsoids = false;

enum class GameState {
    intro,
    playing,
    gameOver
};
GameState currState = GameState::intro;

void changeStateToIntro()
{
    cout << "Going back to intro!" << endl;
    currState = GameState::intro;
    meteorManager.clearMeteors();
    bird.startFlying();
}
void changeStateToPlaying()
{
    cout << "Starting game!" << endl;
    currState = GameState::playing;
}
void changeStateToGameOver()
{
    cout << "Game over!" << endl;
    cout << "Press r to get back to intro!" << endl;
    currState = GameState::gameOver;
}


float getTimeFactorBetweenUpdates() {
    //cout << diffTime << endl;
    float timeseconds = (float)diffTime/1000;
    float factor = 1.0f;
    return timeseconds*factor;
}

void collectGameObjects () {
    gameObjects.clear();
    gameObjects.push_back(&bird);
    laserManager.appendLasers(&gameObjects);
    meteorManager.appendMeteors(&gameObjects);
}

void updateObjects() {
    bird.update(getTimeFactorBetweenUpdates(), camera.getHeightOfView());
    if(currState == GameState::intro)
    {
        if (bird.getOrientation()[3][1] < -1 && bird.getVelocity()[1] < 0) {
                bird.jump();
        }
    }

    laserManager.updateLasers(getTimeFactorBetweenUpdates());
    if(currState != GameState::intro)
    {
        meteorManager.spawnMeteorsRandomly(getTimeFactorBetweenUpdates(), &bird);
    }
}

void testCollisions() {
    collectGameObjects();
    for(int i=0; i<((int)gameObjects.size())-1; i++) {
        for(size_t j=i+1; j<gameObjects.size(); j++) {
            gameObjects[i]->testCollision(gameObjects[j]);
        }
    }
}

void handleCollisions() {
    if(bird.getCollided() || bird.touchingGround(camera.getHeightOfView())) {
        changeStateToGameOver();
        bird.setCollided(false);
    }
    laserManager.cleanupLasers(&camera);
    meteorManager.cleanupMeteors(&bird);
}

void drawSkyBox() {
    glUseProgram(skyboxShading.getName());

    glDisable(GL_DEPTH_TEST);

    glBindTexture( GL_TEXTURE_2D, skyboxTexture.getName() );

    float offset = camera.getOrientation()[3][0]/50.0f;

    glUniform1f(skyboxOffsetLocation, offset);
    glUniform1i(skyboxSamplerLocation, 0/*GL_TEXTURE0*/);

    skyboxMesh.draw();

    glEnable(GL_DEPTH_TEST);
}

void drawEllipsoids () {
    if(drawBoundingEllipsoids) {
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

void renderFrame()
{
    //Clear the render buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);

    // make camera move with bird
    camera.offsetPosition(glm::vec3(1.0f, 0.0f, 0.0f)*bird.getFlyVelocity()*getTimeFactorBetweenUpdates());

    drawSkyBox();

    //Select which shader program we use
    glUseProgram(phongShading.getName());

    // send the camera matrix to the shader
    glUniformMatrix4fv(phongCameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera.getOrientation()));

    //send the texture selection to the shader
    glUniform1i(phongSamplerLocation, 0/*GL_TEXTURE0*/);

    float laserPositions[30];
    laserManager.fillLaserPositionsArray(laserPositions, 10);
    glUniform3fv(phongLazorPositionsLocation, 10, laserPositions);

    bird.draw(phongOrientationMatrixLocation);
    meteorManager.drawMeteors(phongOrientationMatrixLocation);

    glUseProgram(flatShading.getName());
    glUniformMatrix4fv(flatCameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera.getOrientation()));

    laserManager.drawLasers(flatOrientationMatrixLocation);
    drawEllipsoids();

    //swap the renderbuffer with the screenbuffer
    glutSwapBuffers();
}

static void updateGame(void)
{
    int glutElapsedTime = glutGet(GLUT_ELAPSED_TIME);
    diffTime = glutElapsedTime - lastTime;
    lastTime = glutElapsedTime;

    updateObjects();

    renderFrame();

    testCollisions();

    handleCollisions();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    camera.setViewportAspectRatio(ar);

    glViewport(0, 0, width, height);
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
         case 'l':
            drawBoundingEllipsoids = !drawBoundingEllipsoids;
            break;
        case ' ':
            if(currState == GameState::intro)
            {
                changeStateToPlaying();
            }
            if (currState == GameState::playing && jumped == false && bird.getOrientation()[3][1] < camera.getHeightOfView()*0.85) {
                bird.jump();
                jumped = true;
            }
            break;
        case 'p':
            if (currState != GameState::gameOver)
            {
                laserManager.shootLaser(&bird);
            }
            break;
        case 'r':
            if (currState == GameState::gameOver)
            {
                changeStateToIntro();
            }
            break;
    }

    glutPostRedisplay();
}

bool setupOpenGL (int argc, char *argv[]) {
        //Initialize GLUT and create window in which we can draw using OpenGL
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
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
    glutDisplayFunc(updateGame);
    glutKeyboardFunc(key);
    glutKeyboardUpFunc(keyUp);
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

    bool result = phongShading.setupShaders(phongVertexShaderName, phongFragmentShaderName);
    assert(result);
    phongCameraMatrixLocation = glGetUniformLocation(phongShading.getName(), "camera");
    assert(phongCameraMatrixLocation != 0xFFFFFFFF);
    phongSamplerLocation = glGetUniformLocation(phongShading.getName(), "textureSampler");
    assert(phongSamplerLocation != 0xFFFFFFFF);
    phongOrientationMatrixLocation = glGetUniformLocation(phongShading.getName(), "orientation");
    assert(phongOrientationMatrixLocation != 0xFFFFFFFF);
    phongLazorPositionsLocation = glGetUniformLocation(phongShading.getName(), "lazorPos");
    assert(phongLazorPositionsLocation != 0xFFFFFFFF);

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
    flappyTexture.load("textures/camoFlap.png");
    sphereTexture.load("textures/white.png");
    skyboxTexture.load("textures/skybox.png");

    meteorManager.loadMeteorTexture();
    laserManager.loadLaserTextures();
}

void setupModels() {
    Geometry geomSphere, geomSkybox, geomFlappy;

    //Make geometry
    geomSphere.makeSphere(20, 20);
    geomSkybox.makeQuad();

    geomFlappy.loadOBJ("models/FlappyDerpinator.obj", true);

    sphereMesh.makeMesh(geomSphere);
    skyboxMesh.makeMesh(geomSkybox);
    flappyMesh.makeMesh(geomFlappy);

    bird.setMesh(&flappyMesh);
    bird.setTexture(&flappyTexture);
    bird.startFlying();

    laserManager.loadLaserMesh();
    meteorManager.makeMeteorMeshes(10);

    //Geometry instances can go out of scope because the geometry has been copied to the video card
}

//Program entry point
int main(int argc, char *argv[])
{
    setupOpenGL(argc, argv);
    setupShaders();
    setupTextures();
    setupModels();

    camera = Camera(glm::vec3(0.0f,0.0f,24.0f));
    //camera.setPosition(glm::vec3(0.0f,0.0f,24.0f));

    //seed the random number generator
    srand(time(0));

    lastTime = glutGet(GLUT_ELAPSED_TIME);
    //Start the GLUT loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
