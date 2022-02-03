#include "Entity.h"

const unsigned int Entity::getVAO() const
{
    return VAO;
}

const unsigned int Entity::getVBO() const
{
    return VBO;
}

unsigned int* Entity::setVAO()
{
    return &VAO;
}

unsigned int* Entity::setVBO()
{
    return &VBO;
}

const glm::vec3 Entity::getPosition() const
{
    return position;
}
