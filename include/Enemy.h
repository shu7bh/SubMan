#pragma once
#include <glm/glm.hpp>
#include "global.h"
#include <random>
#include "Entity.h"

struct Enemy
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 enemyPos;
    float speed;

    unsigned int VBO, VAO;
    Direction dir;

    std::uniform_real_distribution<float> s;

    Enemy(glm::vec3 enemyPos, Direction(dir)) :
        enemyPos(enemyPos), dir(dir), s(0.1, 0.5)
    {
        speed = s(gen);
    }

    void moveEnemy();
};

