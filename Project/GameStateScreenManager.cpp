#include "GameStateScreenManager.hpp"

void GameStateScreenManager::drawQuad(GLuint flatOrientationMatrixLocation, Texture& tex, const glm::vec3& pos, float height, float screenAspectRatio, float texAspectRatio) {
    // Load texture at frame
    glBindTexture( GL_TEXTURE_2D, tex.getName() );

    //send the orientation matrix to the shader
    glm::mat4 orientation;
    orientation *= glm::translate(pos);
    orientation *= glm::scale(glm::vec3(height*(texAspectRatio/screenAspectRatio), height, 1));
    glUniformMatrix4fv(flatOrientationMatrixLocation, 1, GL_FALSE, glm::value_ptr(orientation));

    quadMesh.draw();
}

void GameStateScreenManager::init() {
    Geometry quadGeometry;
    quadGeometry.makeQuad();
    quadMesh.makeMesh(quadGeometry);

    bannerTexture.load("textures/LaserBird.png");
    gameOverTexture.load("textures/GameOver.png");
    controlsTexture.load("textures/Controls.png");
    restartTexture.load("textures/RestartButton.png");
}

void GameStateScreenManager::draw (GLuint flatOrientationMatrixLocation, GameState gameState, float screenAspectRatio) {
    const float pixelsToHeight = 0.15/222;
    if(gameState == GameState::intro)
    {
        drawQuad(flatOrientationMatrixLocation, bannerTexture, glm::vec3(0, 0.6, 0), pixelsToHeight*222, screenAspectRatio, 1261.0f/222.0f);
        drawQuad(flatOrientationMatrixLocation, controlsTexture, glm::vec3(0, -0.6, 0), pixelsToHeight*367, screenAspectRatio, 1208.0f/367.0f);
    }
    if(gameState == GameState::gameOver)
    {
        drawQuad(flatOrientationMatrixLocation, gameOverTexture, glm::vec3(0, 0.6, 0), pixelsToHeight*210, screenAspectRatio, 1357.0f/210.0f);
        drawQuad(flatOrientationMatrixLocation, restartTexture, glm::vec3(0, -0.6, 0), pixelsToHeight*159, screenAspectRatio, 553.0f/163.0f);
    }
}
