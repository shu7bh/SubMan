#pragma once
#include <glm/glm.hpp>
#include "global.h"

class Entity
{
    glm::vec3 position;
    unsigned int VBO, VAO;

public:
    Entity();
    Entity(const glm::vec3& position) : position(position) { }

    unsigned int getVAO();
    unsigned int getVBO();

    unsigned int* setVAO();
    unsigned int* setVBO();

    glm::vec3 getPosition();
};
