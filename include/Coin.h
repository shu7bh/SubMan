#pragma once
#include "Entity.h"
#include "global.h"
#include <vector>

class Coin : public Entity
{
    float up, down, left, right;
    static int coinCount;
    static int coinsCollected;

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

    static void setCoinCount(const int count) { coinCount = count; }
    static void incrementCoinsCollected() { coinsCollected++; }

    static bool allCoinsCollected() { return coinsCollected == coinCount; }

    static void generate();
    static void eraseCoin(const int index);
    static void clearCoins();
    static bool isCollidingWithCoins(const glm::vec3& pos, const float d, const bool delFlag = false);
    static const int getCoinCount() { return coinCount; }

    static std::vector<Coin> coins;

    static constexpr float R = 0.8f, G = 0.8f, B = 0.8f;
    static constexpr float COINSIZE = 0.015f;
    static constexpr int COINSCORE = 10;
};
