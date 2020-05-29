#ifndef HPP_GAMESTATESCREENMANAGER
#define HPP_GAMESTATESCREENMANAGER

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "Geometry.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "GameState.hpp"

class GameStateScreenManager
{
    Mesh quadMesh;
    Texture bannerTexture, gameOverTexture, controlsTexture, restartTexture;


    void drawQuad(GLuint flatOrientationMatrixLocation, Texture& tex, const glm::vec3& pos, float height, float screenAspectRatio, float texAspectRatio);
public:
    void init();

    void draw (GLuint flatOrientationMatrixLocation, GameState gameState, float screenAspectRatio);
};

#endif // HPP_GAMESTATESCREENMANAGER
