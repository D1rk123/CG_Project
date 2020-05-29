#include "MeteorManager.hpp"

void MeteorManager::spawnMeteor(glm::vec3 position) {
    int index = rand()%meteorMeshes.size();

    meteors.push_back(GameObject(&(meteorMeshes[index]), &meteorTexture));
    meteors.back().transform(glm::translate(position));
}

void MeteorManager::spawnMeteorsRandomly(float updateTime, FlappyBird* bird) {
    countdown += updateTime;
    float randSpawnTime;
    int randNum = rand()%100;
    randSpawnTime = 0.5 + (0.5*((meteors.size() + randNum) % 3));
    //cout << randSpawnTime << endl;
    if(countdown > randSpawnTime) {
        int randomY = (rand()%19) - 9;
        glm::vec3 randomPos = glm::vec3(bird->getOrientation()[3][0]+40.0f, randomY, 0.0f);
        if(meteors.size() < maxAmountOfMeteors) {
            spawnMeteor(randomPos);
            countdown = rand()%3;
        }
    }
}

void MeteorManager::cleanupMeteors(FlappyBird* bird) {
    for(auto iter = meteors.begin(); iter != meteors.end(); iter++) {
        if(iter->getCollided() ||
           iter->checkAwayFromFlappy(bird->getOrientation()[3][0])) {
            iter = meteors.erase(iter);
        }
    }
}
void MeteorManager::clearMeteors(){
    meteors.clear();
}
void MeteorManager::draw (GLuint phongOrientationMatrixLocation) {
    for(auto iter = meteors.begin(); iter != meteors.end(); iter++) {
        iter->draw(phongOrientationMatrixLocation);
    }
}

void MeteorManager::makeMeteorMeshes(int amount) {
    meteorMeshes.reserve(amount);
    for(int i=0; i<amount; i++) {
        Geometry geomMeteor;
        geomMeteor.makeRandomMeteor(15, 15, 12, 0.05f);
        meteorMeshes.push_back(Mesh(geomMeteor));
        geomMeteor.remove();
    }
}

void MeteorManager::loadMeteorTexture() {
    meteorTexture.load("textures/meteor.png");
}

void MeteorManager::appendMeteors(std::vector<GameObject*>* gameObjects) {
    for(auto iter = meteors.begin(); iter != meteors.end(); ++iter) {
        gameObjects->push_back(&(*iter));
    }
}
