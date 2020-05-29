#ifndef HPP_TEXTURE
#define HPP_TEXTURE
#include <iostream>
#include <string>
#include <cassert>
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilut.h>

using std::cout;
using std::endl;
using std::string;
/**
 @brief Loads 2D textures from files into OpenGL.
*/
class Texture
{
    GLuint name;
    bool loaded;

    public:
    bool load(string fileName);

    Texture ();
    Texture (std::string FileName);

    inline bool isLoaded();

    GLuint getName();
    inline void remove();

    ~Texture();
};
#endif
