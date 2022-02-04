#pragma once
#include <string>

struct Texture
{
    unsigned int VAO, VBO, texture;
    std::string path;

    Texture(const std::string& path) : path(path) { }
    void Init(const float x, const float y, const float XblockSize, const float YblockSize);
    void renderText();
};
