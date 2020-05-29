#include "Texture.hpp"

bool Texture::load(string fileName)
{
    if(loaded)
    {
        cout << "Warning: An already loaded texture is loaded again." << endl;
        remove();
    }

    ILuint imageName;
    // DevIL's image functions are much like openGL's texture functions
    // generate a image name
    ilGenImages(1, &imageName);
    // use that image name (all DevIL functions after this point are applied on the newly made image name)
    ilBindImage(imageName);

    // load an image from the file
    if(ilLoadImage(fileName.c_str()) == IL_FALSE)
    {
        ilDeleteImages(1, &imageName);
        cout << "Error: The file is not found." << endl;
        return false;
    }
    // convert the image to a format openGL can use
    /*ILenum ilColorType;
    if (alpha)
        ilColorType = IL_RGBA;
    else
        ilColorType = IL_RGB*/

    if(ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE) == IL_FALSE)
    {
        ilDeleteImages(1, &imageName);
        cout << "Error: Converting the texture is impossible." << endl;
        return false;
    }

    // generate a texture name
    glGenTextures(1,&name);
    // use that texture name
    glBindTexture(GL_TEXTURE_2D, name);

    // load the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

    //Set texture filtering parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    // because we copied the image we can delete the original data
    ilDeleteImages(1, &imageName);

    //cout << "Loaded texture from file: " << fileName << endl;
    loaded = true;
    return true;
}

Texture::Texture ()
{
    loaded = false;
}
Texture::Texture (std::string FileName)
{
    loaded = false;
    load(FileName);
}

inline bool Texture::isLoaded()
{
    return loaded;
}

GLuint Texture::getName()
{
    assert(loaded == true);
    return name;
}
inline void Texture::remove()
{
    if (loaded)
    {
        glDeleteTextures(1, &name);
        loaded = false;
    }
}

Texture::~Texture()
{
    remove();
}
