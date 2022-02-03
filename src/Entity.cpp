#include "Entity.h"

unsigned int Entity::getVAO()
{
    return VAO;
}

unsigned int Entity::getVBO()
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

glm::vec3 Entity::getPosition()
{
    return position;
}
