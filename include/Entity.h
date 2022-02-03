#pragma once
#include <glm/glm.hpp>
#include "global.h"

class Entity
{
    glm::vec3 position;
    unsigned int VBO, VAO;

public:
    Entity() {}
    Entity(const glm::vec3& position) : position(position) { }

    const unsigned int getVAO() const;
    const unsigned int getVBO() const;

    unsigned int* setVAO();
    unsigned int* setVBO();

    const glm::vec3 getPosition() const;
};
