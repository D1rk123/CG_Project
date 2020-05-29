#ifndef HPP_METEORMANAGER
#define HPP_METEORMANAGER

#include <list>
#include <vector>

#include <glm/glm.hpp>

#include "GameObject.hpp"
#include "FlappyBird.hpp"
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

    void spawnMeteor(glm::vec3 position);
    void spawnMeteorsRandomly(float updateTime, FlappyBird* bird);

    void cleanupMeteors(FlappyBird* bird);
    void clearMeteors();

    void draw (GLuint phongOrientationMatrixLocation);

    void makeMeteorMeshes(int amount);
    void loadMeteorTexture();

    void appendMeteors(std::vector<GameObject*>* gameObjects);
};

#endif
