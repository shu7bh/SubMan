#include "Colliding.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool isCollidingWithObstacles(glm::vec3 p, float d)
{
    for (int i = 0; i < upObstacles.size(); ++i)
        if (p.y + d >= downObstacles[i] && p.y - d <= upObstacles[i] && p.x + d >= leftObstacles[i] && p.x - d <= rightObstacles[i])
            return true;

    return false;
}

bool isColidingWithMainWalls(glm::vec3 p, float d)
{
    if (p.x + d > RIGHTWALL || p.x - d < LEFTWALL || p.y + d > UPWALL || p.y - d < DOWNWALL)
        return true;
    return false;
}

bool isCollidingWithEnemies(glm::vec3 p, float d, glm::vec3 t)
{
    auto a = p - t;
    for (int i = 0; i < upEnemies.size(); ++i)
        if (a.y + d == upEnemies[i] && a.y - d == downEnemies[i] && a.x - d == leftEnemies[i] && a.x + d == rightEnemies[i]);
        else if (p.y + d >= downEnemies[i] && p.y - d <= upEnemies[i] && p.x + d >= leftEnemies[i] && p.x - d <= rightEnemies[i])
            return true;

    return false;
}

bool isCollidingWithCoins(glm::vec3 p, float d)
{
    for (int i = 0; i < upCoins.size(); ++i)
        if (p.y + d >= downCoins[i] && p.y - d <= upCoins[i] && p.x + d >= leftCoins[i] && p.x - d <= rightCoins[i])
            return true;

    return false;
}

bool isColliding(glm::vec3 p, float d)
{
    if (isColidingWithMainWalls(p, d))
        return true;
    if (isCollidingWithObstacles(p, d))
        return true;

    return false;
}

bool isCollidingWithAll(glm::vec3 p, float d, glm::vec3 t)
{
    if (isColliding(p, d) || isCollidingWithEnemies(p, d, t) || isCollidingWithCoins(p, d))
        return true;
    return false;
}


