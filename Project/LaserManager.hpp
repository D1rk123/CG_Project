#ifndef HPP_LASERMANAGER
#define HPP_LASERMANAGER

#include <list>
#include <vector>

#include <glm/glm.hpp>

#include "GameObject.hpp"
#include "FlappyBird.hpp"
#include "Laser.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

class LaserManager
{
    std::list<Laser> lasers;
    Texture laserTextures[3];
    Mesh laserMesh;
public:

    void shootLaser(FlappyBird* bird);

    void updateLasers(float timeElapsed);

    void cleanupLasers(Camera* camera);

    void draw (GLuint flatOrientationMatrixLocation);

    void fillLaserPositionsArray(float* posArray, int numLasersInArray);

    void loadLaserTextures();

    void loadLaserMesh();

    void appendLasers(std::vector<GameObject*>* gameObjects);
};

#endif
