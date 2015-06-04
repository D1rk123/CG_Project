#ifndef HPP_SHADER
#define HPP_SHADER
#include <fstream>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

class ShaderHandler {
    GLuint shaderProgram, vertexShader, fragmentShader
    const char * loadFile(const char* filename)
    {
        std::ifstream file(filename);
        if(!file.is_open())
        {
            cout << "Error opening file: " << filename << endl;
            return NULL;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str().c_str();
    }

    bool addShader(GLuint* shader, GLenum type, const char* filename, GLuint program)
    {
        *shader = glCreateShader(type);

        const GLchar* shaderData = loadFile(filename);
        if (shaderData == NULL)
            return false;
        glShaderSource(*shader, 1, &shaderData, NULL);
        glCompileShader(*shader);

        GLint success;
        glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(*shader, sizeof(infoLog), NULL, infoLog);
            cout << "Error compiling shader: " << filename << ":" << endl << infoLog << endl;
            return false;
        }

        glAttachShader(program, *shader);
        return true;
    }
    public:
    bool setupShaders(vertexShaderName, fragmentShaderName)
    {
        shaderProgram = glCreateProgram();
        addShader(&vertexShader, GL_VERTEX_SHADER, vertexShaderName, shaderProgram);
        addShader(&fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderName, shaderProgram);

        glLinkProgram(shaderProgram);

        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (success == 0) {
            GLchar errorLog[1024];
            glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
            cout << "Error linking shader program: " << errorLog << endl;
            return false;
        }

        glValidateProgram(shaderProgram);
        return true;
    }

    GLuint getShaderProgram()
    {
        return shaderProgram;
    }
};
#endif // HPP_SHADER
