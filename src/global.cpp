#include "global.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::mat4 model = glm::mat4(1);

GameMode gm = GameMode::PAUSE;
GameState gs = GameState::LEVEL1;

int ENEMYCOUNT = 5;
int WALLCOUNT = 20;

int totalScore = 0;
bool isDark = false;

std::random_device rand_dev;
std::mt19937 gen(rand_dev());

glm::vec3 playerPos;
float doorPos;

std::vector<float> upObstacles;
std::vector<float> downObstacles;
std::vector<float> leftObstacles;
std::vector<float> rightObstacles;

std::vector<float> upEnemies;
std::vector<float> downEnemies;
std::vector<float> leftEnemies;
std::vector<float> rightEnemies;
