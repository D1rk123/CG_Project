#ifndef HPP_SHADERHANDLER
#define HPP_SHADERHANDLER
#include <fstream>
#include <iostream>
#include <sstream>

#include <GL/glew.h>

using std::cout;
using std::endl;

/**
 @brief Class for loading vertex and fragment shaders into a shader program
*/
class ShaderProgram {
    GLuint name, vertexShader, fragmentShader;

    const std::string loadFile(const char* filename);

    bool addShader(GLuint* shader, GLenum type, const char* filename, GLuint program);

    public:
    bool setupShaders(const char* vertexShaderName, const char* fragmentShaderName);

    GLuint getName();
};
#endif // HPP_SHADERHANDLER
