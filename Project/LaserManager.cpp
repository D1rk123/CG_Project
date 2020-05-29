#include "LaserManager.hpp"

void LaserManager::shootLaser(FlappyBird* bird)
{
    lasers.push_front(Laser(bird->getOrientation(), bird->getVelocity(), &laserMesh));
}

void LaserManager::updateLasers(float timeElapsed) {
    for(auto iter = lasers.begin(); iter != lasers.end(); ++iter) {
        iter->update(timeElapsed, laserTextures);
    }
}

void LaserManager::cleanupLasers(Camera* camera) {
    for(auto iter = lasers.begin(); iter != lasers.end(); ++iter) {
        if(iter->getCollided() ||
           iter->checkOutsideOfView(camera->getOrientation(), 5.0f)) {
            iter = lasers.erase(iter);
        }
    }
}

void LaserManager::draw (GLuint flatOrientationMatrixLocation) {
    for(auto iter = lasers.begin(); iter != lasers.end(); ++iter) {
        iter->draw(flatOrientationMatrixLocation);
    }
}

void LaserManager::fillLaserPositionsArray(float* posArray, int numLasersInArray) {
    int i=0;
    for(auto iter = lasers.begin(); iter != lasers.end() && i<numLasersInArray; ++iter) {
        posArray[i*3] = iter->getOrientation()[3][0];
        posArray[i*3+1] = iter->getOrientation()[3][1];
        posArray[i*3+2] = iter->getOrientation()[3][2];
        i++;
    }
    while(i<numLasersInArray) {
        posArray[i*3] = -100000.0f;
        posArray[i*3+1] = -100000.0f;
        posArray[i*3+2] = -100000.0f;
        i++;
    }
}

void LaserManager::loadLaserTextures()
{
    laserTextures[0].load("textures/lazor0.png");
    laserTextures[1].load("textures/lazor1.png");
    laserTextures[2].load("textures/lazor2.png");
}

void LaserManager::loadLaserMesh()
{
    Geometry geomLaser;
    geomLaser.loadOBJ("models/lazor.obj", true);
    laserMesh.makeMesh(geomLaser);

    laserMesh.setEllipsoid(BoundingEllipsoid(glm::translate(glm::vec3(0.5f, 0.0f, 0.0f))*glm::scale(glm::vec3(1.054378f, 0.211248f, 0.211248f)), 1.054378f));
}

void LaserManager::appendLasers(std::vector<GameObject*>* gameObjects) {
    for(auto iter = lasers.begin(); iter != lasers.end(); ++iter) {
        gameObjects->push_back(&(*iter));
    }
}
