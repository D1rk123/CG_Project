#include "ShaderProgram.hpp"

const std::string ShaderProgram::loadFile(const char* filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        cout << "Error opening file: " << filename << endl;
        return NULL;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool ShaderProgram::addShader(GLuint* shader, GLenum type, const char* filename, GLuint program)
{
    *shader = glCreateShader(type);

    const std::string shaderString = loadFile(filename);
    const GLchar* shaderData = shaderString.c_str();
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

bool ShaderProgram::setupShaders(const char* vertexShaderName, const char* fragmentShaderName)
{
    name = glCreateProgram();
    addShader(&vertexShader, GL_VERTEX_SHADER, vertexShaderName, name);
    addShader(&fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderName, name);

    glLinkProgram(name);

    GLint success;
    glGetProgramiv(name, GL_LINK_STATUS, &success);
    if (success == 0) {
        GLchar errorLog[1024];
        glGetProgramInfoLog(name, sizeof(errorLog), NULL, errorLog);
        cout << "Error linking shader program: " << errorLog << endl;
        return false;
    }

    glValidateProgram(name);
    return true;
}

GLuint ShaderProgram::getName()
{
    return name;
}
