#include "Enemy.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Colliding.h"

void Enemy::moveEnemy()
{
    static std::uniform_int_distribution<int> d(0, 4);
    bool moved = false;

    if (gm == GameMode::PLAYING)
    {
        auto tempPos = speed * deltaTime * glm::normalize(playerPos - enemyPos);

        if (!isCollidingWithAll(enemyPos + tempPos, ENEMYSIZE, tempPos))
        {
            this->model = glm::translate(this->model, tempPos);
            enemyPos += tempPos;
            moved = true;
        }

        for (int ct = 0; !moved && ct < 10; ++ct)
        {
            switch (dir)
            {
            case Direction::UP:
                tempPos = speed * deltaTime * glm::normalize(glm::vec3(0, 1, 0));
                if (!isCollidingWithAll(enemyPos + tempPos, ENEMYSIZE, tempPos))
                {
                    this->model = glm::translate(this->model, tempPos);
                    enemyPos += tempPos;
                    moved = true;
                }
                break;

            case Direction::DOWN:
                tempPos = speed * deltaTime * glm::normalize(glm::vec3(0, -1, 0));
                if (!isCollidingWithAll(enemyPos + tempPos, ENEMYSIZE, tempPos))
                {
                    this->model = glm::translate(this->model, tempPos);
                    enemyPos += tempPos;
                    moved = true;
                }
                break;

            case Direction::LEFT:
                tempPos = speed * deltaTime * glm::normalize(glm::vec3(-1, 0, 0));
                if (!isCollidingWithAll(enemyPos + tempPos, ENEMYSIZE, tempPos))
                {
                    this->model = glm::translate(this->model, tempPos);
                    enemyPos += tempPos;
                    moved = true;
                }
                break;

            case Direction::RIGHT:
                tempPos = speed * deltaTime * glm::normalize(glm::vec3(1, 0, 0));
                if (!isCollidingWithAll(enemyPos + tempPos, ENEMYSIZE, tempPos))
                {
                    this->model = glm::translate(this->model, tempPos);
                    enemyPos += tempPos;
                    moved = true;
                }
                break;
            }

            if (moved)
                continue;

            int a;
            do
                a = d(gen);
            while (a == static_cast<int>(dir));

            dir = static_cast<Direction>(a);
        }
    }

    upEnemies.push_back(enemyPos.y + ENEMYSIZE);
    downEnemies.push_back(enemyPos.y - ENEMYSIZE);
    leftEnemies.push_back(enemyPos.x - ENEMYSIZE);
    rightEnemies.push_back(enemyPos.x + ENEMYSIZE);
}
