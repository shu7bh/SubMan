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

# define radius 1.0f
#define PI 3.141592653589793

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
glm::mat4 model = glm::mat4(1);
// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1400;

constexpr float Z = 0.0f;
constexpr float UPFLOOR = 0.4f;
constexpr float DOWNFLOOR = -UPFLOOR;
constexpr float RIGHTFLOOR = 0.8f;
constexpr float LEFTFLOOR = -RIGHTFLOOR;

constexpr float FLOORCOLOR = 0.17f;
constexpr float WALLCOLOR = 0.30f;
constexpr float PLAYERCOLORR = 1.0f;
constexpr float PLAYERCOLORG = 1.0f;
constexpr float PLAYERCOLORB = 0.0f;

constexpr float ENEMYCOLORR = 0.0f;
constexpr float ENEMYCOLORG = 0.0f;
constexpr float ENEMYCOLORB = 1.0f;

constexpr float UPWALL = 0.35f;
constexpr float DOWNWALL = -UPWALL;
constexpr float RIGHTWALL = 0.75f;
constexpr float LEFTWALL = -RIGHTWALL;
constexpr float WALLSIZE = 0.02f;
constexpr float PLAYERSIZE = 0.03f;
constexpr float ENEMYSIZE = 0.02f;

constexpr int ENEMYCOUNT = 5;
constexpr int WALLCOUNT = 4;

bool pause = true;
bool gameOver = false;

std::random_device rand_dev;
std::mt19937 gen(rand_dev());

glm::vec3 playerPos;

std::vector<float> upObstacles;
std::vector<float> downObstacles;
std::vector<float> leftObstacles;
std::vector<float> rightObstacles;

std::vector<float> upEnemies;
std::vector<float> downEnemies;
std::vector<float> leftEnemies;
std::vector<float> rightEnemies;


bool isCollidingWithObstacle(glm::vec3 p, float d)
{
    for (int i = 0; i < upObstacles.size(); ++i)
        if (p.y + d >= downObstacles[i] && p.y - d <= upObstacles[i] && p.x + d >= leftObstacles[i] && p.x - d <= rightObstacles[i])
            return true;

    return false;
}

bool isColidingWithMainWalls(glm::vec3 p, float d)
{
    if (p.x + d >= RIGHTWALL || p.x - d <= LEFTWALL)
        return true;
    if (p.y + d >= UPWALL || p.y - d <= DOWNWALL)
        return true;
    return false;
}

bool isColliding(glm::vec3 p, float d)
{
    if (isColidingWithMainWalls(p, d))
        return true;
    if (isCollidingWithObstacle(p, d))
        return true;

    return false;
}

struct Enemy
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 enemyPos;
    float speed = 0.5f;

	unsigned int VBO, VAO;
    Direction dir;

    Enemy(glm::vec3 enemyPos, Direction(dir)) :
        enemyPos(enemyPos), dir(dir) { }

    void moveEnemy();
};

void Enemy::moveEnemy()
{
    static std::uniform_real_distribution<float> direction(0.0f, 1.0f);

    bool moved = false;
    int ct = 0;

    while (!moved && !pause)
    {
        float direc = direction(gen);
        if (!ct)
        {
            if (direc < 0.96f);
            else if (direc < 0.97f)
                dir = Direction::UP;
            else if (direc < 0.98f)
                dir = Direction::DOWN;
            else if (direc < 0.99f)
                dir = Direction::LEFT;
            else
                dir = Direction::RIGHT;
        }
        else
        {
            Direction temp;
            if (direc < 0.25f)
                temp = Direction::UP;
            else if (direc < 0.5f)
                temp = Direction::DOWN;
            else if (direc < 0.75f)
                temp = Direction::LEFT;
            else
                temp = Direction::RIGHT;

            if (temp != dir)
                dir = temp;
            else
                continue;
        }

        ct++;

        glm::vec3 tempPos;
        switch (dir)
        {
        case Direction::UP:
            tempPos = glm::vec3(0.0f, speed * deltaTime, 0.0);

            if (!isColliding(enemyPos + tempPos, ENEMYSIZE))
            {
                this->model = glm::translate(this->model, tempPos);
                enemyPos += tempPos;
                moved = true;
            }
            break;
        case Direction::DOWN:
            tempPos = glm::vec3(0.0f, -speed * deltaTime, 0.0);

            if (!isColliding(enemyPos + tempPos, ENEMYSIZE))
            {
                this->model = glm::translate(this->model, tempPos);
                enemyPos += tempPos;
                moved = true;
            }
            break;
        case Direction::LEFT:
            tempPos = glm::vec3(-speed * deltaTime, 0.0f, 0.0);

            if (!isColliding(enemyPos + tempPos, ENEMYSIZE))
            {
                this->model = glm::translate(this->model, tempPos);
                enemyPos += tempPos;
                moved = true;
            }
            break;
        case Direction::RIGHT:
            tempPos = glm::vec3(speed * deltaTime, 0.0f, 0.0);

            if (!isColliding(enemyPos + tempPos, ENEMYSIZE))
            {
                this->model = glm::translate(this->model, tempPos);
                enemyPos += tempPos;
                moved = true;
            }
            break;
        }
    }

    upEnemies.push_back(enemyPos.y + ENEMYSIZE);
    downEnemies.push_back(enemyPos.y - ENEMYSIZE);
    leftEnemies.push_back(enemyPos.x - ENEMYSIZE);
    rightEnemies.push_back(enemyPos.x + ENEMYSIZE);
}


glm::vec3 pos(float d)
{
    std::uniform_real_distribution<float> x(LEFTWALL + d, RIGHTWALL - d);
    std::uniform_real_distribution<float> y(DOWNWALL + d, UPWALL - d);

    return glm::vec3(x(gen), y(gen), Z);
}

glm::vec3 color()
{
    std::uniform_real_distribution<float> r(0.0f, 1.0f);
    std::uniform_real_distribution<float> g(0.0f, 1.0f);
    std::uniform_real_distribution<float> b(0.0f, 1.0f);

    return glm::vec3(r(gen), g(gen), b(gen));
}

bool isCollidingWithEnemies(glm::vec3 p, float d)
{
    for (int i = 0; i < upEnemies.size(); ++i)
        if (p.y + d >= downEnemies[i] && p.y - d <= upEnemies[i] && p.x + d >= leftEnemies[i] && p.x - d <= rightEnemies[i])
            return true;

    return false;
}

std::vector<float> generateObstacles()
{
    std::vector<float> obstacles;
    auto c = color();

    for (int i = 0; i < WALLCOUNT; ++i)
    {
        auto p = pos(WALLSIZE);
        float d = WALLSIZE;

        while (isCollidingWithObstacle(p, d))
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

        while (isCollidingWithObstacle(p, d) && isCollidingWithEnemies(p, d))
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

    auto obstacles = generateObstacles();
    float obstacleArray[obstacles.size()];

    for (int i = 0; i < obstacles.size(); i++)
        obstacleArray[i] = obstacles[i];

    auto enemies = generateEnemies();

    auto p = pos(PLAYERSIZE);
    while (isCollidingWithObstacle(p, PLAYERSIZE) || isCollidingWithEnemies(p, PLAYERSIZE * 5))
        p = pos(PLAYERSIZE);

    playerPos = p;

    float character[] = {
        p.x + PLAYERSIZE, p.y, Z, PLAYERCOLORR, PLAYERCOLORG, PLAYERCOLORB,
        p.x - PLAYERSIZE, p.y, Z, PLAYERCOLORR, PLAYERCOLORG, PLAYERCOLORB,
        p.x, p.y + PLAYERSIZE, Z, PLAYERCOLORR, PLAYERCOLORG, PLAYERCOLORB,

        p.x + PLAYERSIZE, p.y, Z, PLAYERCOLORR, PLAYERCOLORG, PLAYERCOLORB,
        p.x - PLAYERSIZE, p.y, Z, PLAYERCOLORR, PLAYERCOLORG, PLAYERCOLORB,
        p.x, p.y - PLAYERSIZE, Z, PLAYERCOLORR, PLAYERCOLORG, PLAYERCOLORB
    };

	unsigned int VBO, VAO, wallVAO, wallVBO;
    unsigned characterVAO, characterVBO;
    unsigned obstacleVAO, obstacleVBO;

    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &wallVAO);
    glGenVertexArrays(1, &characterVAO);
    glGenVertexArrays(1, &obstacleVAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &wallVBO);
    glGenBuffers(1, &characterVBO);
    glGenBuffers(1, &obstacleVBO);

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


	// You can unbind the VAO afterwards so other VAO calls won't accidentally
	// modify this VAO, but this rarely happens. Modifying other VAOs requires a
	// call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
	// VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// render loop

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        if (isCollidingWithEnemies(playerPos, PLAYERSIZE))
        {
            if (!gameOver)
                std::cout << "Game Over" << std::endl;

            gameOver = true;
            pause = true;
        }

		// render
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();
        ourShader.setMat4("model", model);

        glBindVertexArray(characterVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // render box
        //unsigned int modelLoc;
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

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

        glBindVertexArray(obstacleVAO);
        glDrawArrays(GL_TRIANGLES, 0, obstacles.size() / 6);

        glBindVertexArray(wallVAO);
        glDrawArrays(GL_TRIANGLES, 0, 24);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse
		// moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();

        //modelLoc = glGetUniformLocation(ourShader.ID, "model");
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    }

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    // Unbind next VAO
    glDeleteVertexArrays(1, &wallVAO);
	glDeleteBuffers(1, &wallVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
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
 		glfwSetWindowShouldClose(window, true);

    static bool pauseFlag = false;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (gameOver)
            glfwSetWindowShouldClose(window, true);

        if (!pauseFlag)
            pause = !pause;

        pauseFlag = true;
    }
    else
        pauseFlag = false;

    glm::vec3 tempPos = glm::vec3(0.0f, 0.0f, 0.0f);

    float speed = static_cast<float>(0.5 * deltaTime);
    bool isMoving = true;

    if (!pause)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            tempPos = speed * glm::vec3(0.0f, 1.0f, 0.0f);

            if (!isColliding(playerPos + tempPos, PLAYERSIZE))
                playerPos += tempPos, model = glm::translate(model, tempPos);
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            tempPos = speed * glm::vec3(0.0f, -1.0f, Z);

            if (!isColliding(playerPos + tempPos, PLAYERSIZE))
                playerPos += tempPos, model = glm::translate(model, tempPos);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            tempPos = speed * glm::vec3(-1.0f, 0.0f, Z);

            if (!isColliding(playerPos + tempPos, PLAYERSIZE))
                playerPos += tempPos, model = glm::translate(model, tempPos);
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            tempPos = speed * glm::vec3(1.0f, 0.0f, Z);

            if (!isColliding(playerPos + tempPos, PLAYERSIZE))
                playerPos += tempPos, model = glm::translate(model, tempPos);
        }
    }
}
