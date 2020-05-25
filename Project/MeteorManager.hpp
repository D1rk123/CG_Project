#ifndef HPP_METEORMANAGER
#define HPP_METEORMANAGER

#include <list>
#include <vector>

#include <glm/glm.hpp>

#include "GameObject.hpp"
#include "Mesh.hpp"

class MeteorManager
{
    std::list<GameObject> meteors;
    unsigned int maxAmountOfMeteors;
    std::vector<Mesh> meteorMeshes;
    Texture meteorTexture;
    float countdown;

public:
    MeteorManager(unsigned int maxAmountOfMeteors)
    : maxAmountOfMeteors(maxAmountOfMeteors), countdown(0)
    {

    }

    void spawnMeteor(glm::vec3 position) {
        int index = rand()%meteorMeshes.size();

        meteors.push_back(GameObject(&(meteorMeshes[index]), &meteorTexture));
        meteors.back().transform(glm::translate(position));
    }

    void spawnMeteorsRandomly(float updateTime, FlappyBird* bird) {
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

    void cleanupMeteors(FlappyBird* bird) {
        for(auto iter = meteors.begin(); iter != meteors.end(); iter++) {
            if(iter->getCollided() ||
               iter->checkAwayFromFlappy(bird->getOrientation()[3][0])) {
                iter = meteors.erase(iter);
            }
        }
    }
    void clearMeteors(){
        meteors.clear();
    }
    void draw (GLuint phongOrientationMatrixLocation) {
        for(auto iter = meteors.begin(); iter != meteors.end(); iter++) {
            iter->draw(phongOrientationMatrixLocation);
        }
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

    void loadMeteorTexture() {
        meteorTexture.load("textures/meteor.jpg");
    }

    void appendMeteors(std::vector<GameObject*>* gameObjects) {
        for(auto iter = meteors.begin(); iter != meteors.end(); ++iter) {
            gameObjects->push_back(&(*iter));
        }
    }
};

#endif
