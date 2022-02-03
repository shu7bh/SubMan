#pragma once
#include <vector>
#include <random>
#include <glm/glm.hpp>

enum class Direction : unsigned char
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class GameState : unsigned char
{
    LEVEL1,
    LEVEL2,
    LEVEL3,
    END,
};

enum class GameMode : unsigned char
{
    PAUSE,
    LOST,
    PLAYING,
    WON
};

constexpr unsigned int SCR_WIDTH = 1080;
constexpr unsigned int SCR_HEIGHT = 1080;

constexpr float Z = 0.0f;
constexpr float UPFLOOR = 0.6f;
constexpr float DOWNFLOOR = -UPFLOOR;
constexpr float RIGHTFLOOR = 1.0f;
constexpr float LEFTFLOOR = -RIGHTFLOOR;

constexpr float FLOORCOLOR = 0.17f;
constexpr float WALLCOLOR = 0.30f;
constexpr float PLAYERCOLORR = 1.0f;
constexpr float PLAYERCOLORG = 1.0f;
constexpr float PLAYERCOLORB = 0.0f;

constexpr float ENEMYCOLORR = 0.0f;
constexpr float ENEMYCOLORG = 0.0f;
constexpr float ENEMYCOLORB = 1.0f;

constexpr float COINCOLORR = 0.0f;
constexpr float COINCOLORG = 1.0f;
constexpr float COINCOLORB = 1.0f;

constexpr float DOORCOLORR = 1.0f;
constexpr float DOORCOLORG = 1.0f;
constexpr float DOORCOLORB = 0.5f;

constexpr float UPWALL = 0.55f;
constexpr float DOWNWALL = -UPWALL;
constexpr float RIGHTWALL = 0.95f;
constexpr float LEFTWALL = -RIGHTWALL;
constexpr float WALLSIZE = 0.025f;
constexpr float PLAYERSIZE = 0.02f;
constexpr float ENEMYSIZE = 0.02f;
constexpr float COINSIZE = 0.015f;
constexpr float DOORSIZE = 0.08f;

constexpr int COINSCORE = 10;

extern float deltaTime;
extern float lastFrame;
extern glm::mat4 model;

extern GameMode gm;
extern GameState gs;

extern int ENEMYCOUNT;
extern int WALLCOUNT;
extern int COINCOUNT;

extern int totalScore;
extern bool isDark;

extern int coinsCollected;

extern std::random_device rand_dev;
extern std::mt19937 gen;

extern glm::vec3 playerPos;
extern float doorPos;

extern std::vector<float> upObstacles;
extern std::vector<float> downObstacles;
extern std::vector<float> leftObstacles;
extern std::vector<float> rightObstacles;

extern std::vector<float> upEnemies;
extern std::vector<float> downEnemies;
extern std::vector<float> leftEnemies;
extern std::vector<float> rightEnemies;

extern std::vector<float> upCoins;
extern std::vector<float> downCoins;
extern std::vector<float> leftCoins;
extern std::vector<float> rightCoins;
