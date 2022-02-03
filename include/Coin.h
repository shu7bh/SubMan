#pragma once
#include "Entity.h"
#include "global.h"
#include <vector>

class Coin : public Entity
{
    float up, down, left, right;
public:
    Coin() : Entity() {}
    Coin
    (
        const glm::vec3& position,
        const float up,
        const float down,
        const float right,
        const float left
    )
        : Entity(position), up(up), down(down), right(right), left(left) {}

    bool isColliding(const glm::vec3& pos, const float d) const;
    void draw();

    static void generate();
    static void eraseCoin(const int index);
    static void clearCoins();
    static bool isCollidingWithCoins(const glm::vec3& pos, const float d, const bool delFlag = false);

    static std::vector<Coin> coins;
};
