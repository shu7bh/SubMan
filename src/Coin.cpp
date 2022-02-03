#include "Coin.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Helper.h"
#include "Colliding.h"

std::vector<Coin> Coin::coins;
int Coin::coinsCollected = 0;
int Coin::coinCount = 0;

void Coin::generate()
{
    for (int i = 0; i < coinCount; ++i)
    {
        float d = COINSIZE;
        auto p = pos(d);

        while (isCollidingWithAll(p, d))
            p = pos(d);

        float arr[] =
        {
            p.x + d, p.y, Z, R, G, B,
            p.x - d, p.y, Z, R, G, B,
            p.x, p.y + d, Z, R, G, B,

            p.x + d, p.y, Z, R, G, B,
            p.x - d, p.y, Z, R, G, B,
            p.x, p.y - d, Z, R, G, B
        };

        Coin coin(glm::vec3(p.x, p.y, Z), p.y + d, p.y - d, p.x + d, p.x - d);

        glGenVertexArrays(1, coin.setVAO());
        glGenBuffers(1, coin.setVBO());

        glBindVertexArray(coin.getVAO());
        glBindBuffer(GL_ARRAY_BUFFER, coin.getVBO());
        glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        coins.push_back(coin);
    }
}

bool Coin::isCollidingWithCoins(const glm::vec3& pos, const float d, const bool delFlag)
{
    for (int i = 0; i < coins.size(); ++i)
        if (coins[i].isColliding(pos, d))
        {
            if (delFlag)
                eraseCoin(i), incrementCoinsCollected();

            return true;
        }

    return false;
}

bool Coin::isColliding(const glm::vec3& pos, const float d) const
{
    if (pos.y + d >= down && pos.y - d <= up && pos.x + d >= left && pos.x - d <= right)
        return true;
    else
        return false;
}

void Coin::eraseCoin(const int index)
{
    coins[index] = coins.back();
    coins.pop_back();
}

void Coin::clearCoins()
{
    coins.clear();
    coinsCollected = 0;
}

void Coin::draw()
{
    glBindVertexArray(getVAO());
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
