#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <glad/glad.h>
#include <string>

class TextureLoader
{
public:
    // Loads a texture from a file path and returns its OpenGL ID
    static GLuint Load(const std::string &path);
};

#endif