#pragma once
#include "global.h"

bool isCollidingWithObstacles(glm::vec3 p, float d);
bool isColidingWithMainWalls(glm::vec3 p, float d);
bool isCollidingWithEnemies(glm::vec3 p, float d, glm::vec3 t = glm::vec3(0, 0, 0));
bool isCollidingWithCoins(glm::vec3 p, float d);
bool isColliding(glm::vec3 p, float d);
bool isCollidingWithAll(glm::vec3 p, float d, glm::vec3 t = glm::vec3(0, 0, 0));
