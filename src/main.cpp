#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <random>

#include "shader.h"
#include "global.h"
#include "Enemy.h"
#include "Colliding.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

glm::vec3 pos(float d)
{
    std::uniform_real_distribution<float> x(LEFTWALL + d, RIGHTWALL - d);
    std::uniform_real_distribution<float> y(DOWNWALL + d, UPWALL - d);

    return glm::vec3(x(gen), y(gen), Z);
}

struct Coin
{
    glm::vec3 coinPos;

    unsigned int VBO, VAO;

    Coin(glm::vec3 coinPos) :
        coinPos(coinPos) { }
};

glm::vec3 color()
{
    std::uniform_real_distribution<float> r(0.0f, 1.0f);
    std::uniform_real_distribution<float> g(0.0f, 1.0f);
    std::uniform_real_distribution<float> b(0.0f, 1.0f);

    return glm::vec3(r(gen), g(gen), b(gen));
}

std::vector<float> generateObstacles()
{
    std::vector<float> obstacles;
    auto c = color();

    for (int i = 0; i < WALLCOUNT; ++i)
    {
        auto p = pos(WALLSIZE);
        float d = WALLSIZE;

        while (isCollidingWithObstacles(p, d))
            p = pos(WALLSIZE);

        obstacles.push_back(p.x + d);
        obstacles.push_back(p.y + d);
        obstacles.push_back(p.z);

        obstacles.push_back(c.x);
        obstacles.push_back(c.y);
        obstacles.push_back(c.z);

        obstacles.push_back(p.x - d);
        obstacles.push_back(p.y + d);
        obstacles.push_back(p.z);

        obstacles.push_back(c.x);
        obstacles.push_back(c.y);
        obstacles.push_back(c.z);

        obstacles.push_back(p.x - d);
        obstacles.push_back(p.y - d);
        obstacles.push_back(p.z);

        obstacles.push_back(c.x);
        obstacles.push_back(c.y);
        obstacles.push_back(c.z);

        obstacles.push_back(p.x + d);
        obstacles.push_back(p.y - d);
        obstacles.push_back(p.z);

        obstacles.push_back(c.x);
        obstacles.push_back(c.y);
        obstacles.push_back(c.z);

        obstacles.push_back(p.x + d);
        obstacles.push_back(p.y + d);
        obstacles.push_back(p.z);

        obstacles.push_back(c.x);
        obstacles.push_back(c.y);
        obstacles.push_back(c.z);

        obstacles.push_back(p.x - d);
        obstacles.push_back(p.y - d);
        obstacles.push_back(p.z);

        obstacles.push_back(c.x);
        obstacles.push_back(c.y);
        obstacles.push_back(c.z);

        upObstacles.push_back(p.y + d);
        downObstacles.push_back(p.y - d);
        leftObstacles.push_back(p.x - d);
        rightObstacles.push_back(p.x + d);
    }

    return obstacles;
}

std::vector<Enemy> generateEnemies()
{
    std::vector<Enemy> enemies;
    for (int i = 0; i < ENEMYCOUNT; ++i)
    {
        auto p = pos(ENEMYSIZE);
        float d = ENEMYSIZE;

        while (isCollidingWithAll(p, d))
            p = pos(ENEMYSIZE);

        float arr[] = {
            p.x + d, p.y, Z, ENEMYCOLORR, ENEMYCOLORG, ENEMYCOLORB,
            p.x - d, p.y, Z, ENEMYCOLORR, ENEMYCOLORG, ENEMYCOLORB,
            p.x, p.y + d, Z, ENEMYCOLORR, ENEMYCOLORG, ENEMYCOLORB,

            p.x + d, p.y, Z, ENEMYCOLORR, ENEMYCOLORG, ENEMYCOLORB,
            p.x - d, p.y, Z, ENEMYCOLORR, ENEMYCOLORG, ENEMYCOLORB,
            p.x, p.y - d, Z, ENEMYCOLORR, ENEMYCOLORG, ENEMYCOLORB
        };

        upEnemies.push_back(p.y + d);
        downEnemies.push_back(p.y - d);
        leftEnemies.push_back(p.x - d);
        rightEnemies.push_back(p.x + d);

        static std::uniform_int_distribution<int> direction(0, 4);
        int direc = direction(gen) % 4;
        Direction dir;

        switch (direc)
        {
            case 0:
                dir = Direction::UP;
                break;
            case 1:
                dir = Direction::DOWN;
                break;
            case 2:
                dir = Direction::LEFT;
                break;
            case 3:
                dir = Direction::RIGHT;
                break;
        }

        Enemy enemy(glm::vec3(p.x, p.y, Z), dir);

        glGenVertexArrays(1, &enemy.VAO);
        glGenBuffers(1, &enemy.VBO);

        glBindVertexArray(enemy.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, enemy.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        enemies.push_back(enemy);
    }
    return enemies;
}

std::vector<Coin> generateCoins()
{
    std::vector<Coin> coins;
    for (int i = 0; i < COINCOUNT; ++i)
    {
        float d = COINSIZE;
        auto p = pos(d);

        while (isCollidingWithAll(p, d))
            p = pos(d);

        float arr[] = {
            p.x + d, p.y, Z, COINCOLORR, COINCOLORG, COINCOLORB,
            p.x - d, p.y, Z, COINCOLORR, COINCOLORG, COINCOLORB,
            p.x, p.y + d, Z, COINCOLORR, COINCOLORG, COINCOLORB,

            p.x + d, p.y, Z, COINCOLORR, COINCOLORG, COINCOLORB,
            p.x - d, p.y, Z, COINCOLORR, COINCOLORG, COINCOLORB,
            p.x, p.y - d, Z, COINCOLORR, COINCOLORG, COINCOLORB
        };

        upCoins.push_back(p.y + d);
        downCoins.push_back(p.y - d);
        leftCoins.push_back(p.x - d);
        rightCoins.push_back(p.x + d);

        Coin coin(glm::vec3(p.x, p.y, Z));

        glGenVertexArrays(1, &coin.VAO);
        glGenBuffers(1, &coin.VBO);

        glBindVertexArray(coin.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, coin.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        coins.push_back(coin);
    }
    return coins;
}

int main(int argc, char *argv[])
{

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2D Game", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    Shader ourShader("../src/vertex.shader", "../src/fragment.shader");

    float vertices[] = {
        // FLOOR
        RIGHTFLOOR, UPFLOOR, Z, FLOORCOLOR, FLOORCOLOR, FLOORCOLOR,     // top right
        RIGHTFLOOR, DOWNFLOOR, Z, FLOORCOLOR, FLOORCOLOR, FLOORCOLOR,    // bottom right
        LEFTFLOOR, DOWNFLOOR, Z, FLOORCOLOR, FLOORCOLOR, FLOORCOLOR,   // bottom left

        LEFTFLOOR, DOWNFLOOR, Z, FLOORCOLOR, FLOORCOLOR, FLOORCOLOR,   // bottom left
        LEFTFLOOR, UPFLOOR, Z, FLOORCOLOR, FLOORCOLOR, FLOORCOLOR,    // top left
        RIGHTFLOOR, UPFLOOR, Z, FLOORCOLOR, FLOORCOLOR, FLOORCOLOR     // top right
    };

    // Array for side walls
    float wall[] = {
        // Top wall
        LEFTFLOOR, UPWALL, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        RIGHTFLOOR, UPFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        RIGHTFLOOR, UPWALL, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,

        RIGHTFLOOR, UPFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        LEFTFLOOR, UPWALL, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        LEFTFLOOR, UPFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,

        // Bottom wall
        RIGHTFLOOR, DOWNFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        LEFTFLOOR, DOWNFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        LEFTFLOOR, DOWNWALL, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,

        LEFTFLOOR, DOWNWALL, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        RIGHTFLOOR, DOWNWALL, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        RIGHTFLOOR, DOWNFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,

        // Left wall
        LEFTFLOOR, UPFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        LEFTFLOOR, DOWNFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        LEFTWALL, DOWNFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,

        LEFTWALL, DOWNFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        LEFTWALL, UPFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        LEFTFLOOR, UPFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,

        // Right wall
        RIGHTFLOOR, UPFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        RIGHTFLOOR, DOWNFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        RIGHTWALL, DOWNFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,

        RIGHTWALL, DOWNFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        RIGHTWALL, UPFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR,
        RIGHTFLOOR, UPFLOOR, Z, WALLCOLOR, WALLCOLOR, WALLCOLOR
    };

    unsigned int VBO, VAO, wallVAO, wallVBO;
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &wallVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &wallVBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Binding Next VAO
    glBindVertexArray(wallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall), wall, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (static_cast<int>(gs) != static_cast<int>(GameState::END))
    {
        auto obstacles = generateObstacles();
        auto enemies = generateEnemies();
        auto coins = generateCoins();

        float obstacleArray[obstacles.size()];

        for (int i = 0; i < obstacles.size(); i++)
            obstacleArray[i] = obstacles[i];

        auto p = pos(PLAYERSIZE);
        while (isCollidingWithObstacles(p, PLAYERSIZE) || isCollidingWithEnemies(p, PLAYERSIZE * 9) || isCollidingWithCoins(p, PLAYERSIZE * 2))
            p = pos(PLAYERSIZE);

        playerPos = p;
        doorPos = pos(DOORSIZE).x;

        float doorArray[] = {
            // Door
            doorPos + DOORSIZE, UPFLOOR, Z, DOORCOLORR, DOORCOLORG, DOORCOLORB,
            doorPos - DOORSIZE, UPFLOOR, Z, DOORCOLORR, DOORCOLORG, DOORCOLORB,
            doorPos + DOORSIZE, UPWALL, Z, DOORCOLORR, DOORCOLORG, DOORCOLORB,

            doorPos - DOORSIZE, UPWALL, Z, DOORCOLORR, DOORCOLORG, DOORCOLORB,
            doorPos - DOORSIZE, UPFLOOR, Z, DOORCOLORR, DOORCOLORG, DOORCOLORB,
            doorPos + DOORSIZE, UPWALL, Z, DOORCOLORR, DOORCOLORG, DOORCOLORB
        };

        float character[] = {
            p.x + PLAYERSIZE, p.y, Z, PLAYERCOLORR, PLAYERCOLORG, PLAYERCOLORB,
            p.x - PLAYERSIZE, p.y, Z, PLAYERCOLORR, PLAYERCOLORG, PLAYERCOLORB,
            p.x, p.y + PLAYERSIZE, Z, PLAYERCOLORR, PLAYERCOLORG, PLAYERCOLORB,

            p.x + PLAYERSIZE, p.y, Z, PLAYERCOLORR, PLAYERCOLORG, PLAYERCOLORB,
            p.x - PLAYERSIZE, p.y, Z, PLAYERCOLORR, PLAYERCOLORG, PLAYERCOLORB,
            p.x, p.y - PLAYERSIZE, Z, PLAYERCOLORR, PLAYERCOLORG, PLAYERCOLORB
        };

        unsigned characterVAO, characterVBO;
        unsigned obstacleVAO, obstacleVBO;
        unsigned doorVAO, doorVBO;

        glGenVertexArrays(1, &characterVAO);
        glGenVertexArrays(1, &obstacleVAO);
        glGenVertexArrays(1, &doorVAO);

        glGenBuffers(1, &characterVBO);
        glGenBuffers(1, &obstacleVBO);
        glGenBuffers(1, &doorVBO);

        glBindVertexArray(characterVAO);
        glBindBuffer(GL_ARRAY_BUFFER, characterVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(character), character, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(obstacleVAO);
        glBindBuffer(GL_ARRAY_BUFFER, obstacleVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(obstacleArray), obstacleArray, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(doorVAO);
        glBindBuffer(GL_ARRAY_BUFFER, doorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(doorArray), doorArray, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        // render loop

        while (static_cast<int>(gs) != static_cast<int>(GameState::END) && !glfwWindowShouldClose(window))
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            if (isCollidingWithEnemies(playerPos, PLAYERSIZE))
            {
                if (static_cast<int>(gm) != static_cast<int>(GameMode::LOST) && static_cast<int>(gm) != static_cast<int>(GameMode::PAUSE))
                    std::cout << "Game Over" << std::endl << "Score: " << totalScore << std::endl;
                gm = GameMode::LOST;
            }

            if (isCollidingWithCoins(playerPos, PLAYERSIZE))
            {
                float d = PLAYERSIZE;
                auto p = playerPos;
                for (int i = 0; i < upCoins.size(); ++i)
                    if (p.y + d >= downCoins[i] && p.y - d <= upCoins[i] && p.x + d >= leftCoins[i] && p.x - d <= rightCoins[i])
                    {
                        upCoins.erase(upCoins.begin() + i);
                        downCoins.erase(downCoins.begin() + i);
                        leftCoins.erase(leftCoins.begin() + i);
                        rightCoins.erase(rightCoins.begin() + i);
                        coins.erase(coins.begin() + i);
                        coinsCollected++;
                        totalScore += COINSCORE * ((isDark) ? 2 : 1);
                        std::cout << "Score: " << totalScore << std::endl;
                        break;
                    }
            }

            // Colliding with the door
            if (gm == GameMode::PLAYING)
                if (coinsCollected == COINCOUNT && playerPos.x <= doorPos + DOORSIZE && playerPos.x >= doorPos - DOORSIZE && playerPos.y + PLAYERSIZE >= UPWALL - PLAYERSIZE)
                {
                    std::cout << "Level Completed" << std::endl << "Score: " << totalScore << std::endl;
                    gm = GameMode::WON;
                    break;
                }

            // Clearing the screen
            glClearColor(0.0, 0.0, 0.0, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ourShader.use();

            // Setting the player model
            ourShader.setMat4("model", model);
            ourShader.setBool("isDark", isDark);
            ourShader.setVec3("playerPos", playerPos);

            // Binding the character
            glBindVertexArray(characterVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            // Clearing the enemy positions
            upEnemies.clear();
            downEnemies.clear();
            leftEnemies.clear();
            rightEnemies.clear();

            for (auto& enemy: enemies)
            {
                enemy.moveEnemy();
                ourShader.setMat4("model", enemy.model);

                glBindVertexArray(enemy.VAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            ourShader.setMat4("model", glm::mat4(1));

            for (auto& coin: coins)
            {
                glBindVertexArray(coin.VAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            glBindVertexArray(doorVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindVertexArray(obstacleVAO);
            glDrawArrays(GL_TRIANGLES, 0, obstacles.size() / 6);

            glBindVertexArray(wallVAO);
            glDrawArrays(GL_TRIANGLES, 0, 24);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        if (gs == GameState::END)
            break;
        else if (gs == GameState::LEVEL3)
            gm = GameMode::PAUSE, gs = GameState::END;
        else
            gs = static_cast<GameState> (static_cast<int> (gs) + 1);

        upObstacles.clear();
        downObstacles.clear();
        leftObstacles.clear();
        rightObstacles.clear();

        upEnemies.clear();
        downEnemies.clear();
        leftEnemies.clear();
        rightEnemies.clear();

        upCoins.clear();
        downCoins.clear();
        leftCoins.clear();
        rightCoins.clear();

        coinsCollected = 0;

        ENEMYCOUNT += 1;
        WALLCOUNT += 1;
        COINCOUNT += 1;

        model = glm::mat4(1);
        deltaTime = 0.0f;
        lastFrame = 0.0f;

        isDark = false;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &wallVAO);
    glDeleteBuffers(1, &wallVBO);

    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        gs = GameState::END;
        gm = GameMode::PAUSE;

        return;
    }

    static bool pauseFlag = false;
    static bool darkFlag = false;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (!pauseFlag)
        {
            switch (gm)
            {
                case GameMode::PLAYING:
                    gm = GameMode::PAUSE;
                    break;
                case GameMode::WON:
                    gm = GameMode::PLAYING;
                    if (gs == GameState::END) { return; }

                    break;
                case GameMode::LOST:
                    gm = GameMode::PAUSE;
                    gs = GameState::END;
                    return;
                    break;
                case GameMode::PAUSE:
                    gm = GameMode::PLAYING;
                    break;
            }
        }
        pauseFlag = true;
    }
    else
        pauseFlag = false;

    // Toggle dark mode on/off with l key
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        if (!darkFlag)
            isDark = !isDark;
        darkFlag = true;
    }
    else
        darkFlag = false;

    static glm::vec3 tempPos, lastPos = glm::vec3(0.0f, 0.0f, 0.0f);
    tempPos = glm::vec3(0.0f, 0.0f, 0.0f);

    static float speedx = 0.0f;
    static float speedy = 0.0f;

    float speed = static_cast<float>(0.5 * deltaTime);

    if (gm == GameMode::PLAYING)
    {
        bool move = false;
        auto up = speed * glm::vec3(0.0f, 1.0f, 0.0f);
        auto right = speed * glm::vec3(1.0f, 0.0f, 0.0f);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            if (!isColliding(playerPos + up, PLAYERSIZE))
                tempPos += up, move = true;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && (move = true))
        {
            if (!isColliding(playerPos - up, PLAYERSIZE))
                tempPos -= up, move = true;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && (move = true))
        {
            if (!isColliding(playerPos - right, PLAYERSIZE))
                tempPos -= right, move = true;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && (move = true))
        {
            if (!isColliding(playerPos + right, PLAYERSIZE))
                tempPos += right, move = true;
        }

        if (move && tempPos != glm::vec3(0, 0, 0))
        {
            tempPos = glm::normalize(tempPos);
            tempPos = glm::vec3((speed + speedx) * tempPos.x, (speed + speedy) * tempPos.y, Z);

            if (!isColliding(playerPos + tempPos, PLAYERSIZE))
            {
                playerPos += tempPos, model = glm::translate(model, tempPos);
                if (tempPos.x * lastPos.x > 0)
                    speedx += 0.0002;
                else
                    speedx = 0;

                if (tempPos.y * lastPos.y > 0)
                    speedy += 0.0002;
                else
                    speedy = 0;

            }
            else
            {
                if (tempPos.x * tempPos.x > 0)
                    tempPos.y = 0;
                else
                    tempPos.x = 0;

                if (!isColliding(playerPos + tempPos, PLAYERSIZE))
                {
                    playerPos += tempPos, model = glm::translate(model, tempPos);
                    if (tempPos.x * lastPos.x > 0)
                        speedx += 0.0002;
                    else
                        speedx = 0;

                    if (tempPos.y * lastPos.y > 0)
                        speedy += 0.0002;
                    else
                        speedy = 0;
                }
            }

            lastPos = tempPos;
        }
        else
            lastPos = glm::vec3(0, 0, 0), speedx = speedy = 0.0f;
    }
    else
        lastPos = glm::vec3(0, 0, 0), speedx = speedy = 0.0f;
}
