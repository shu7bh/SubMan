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


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
glm::mat4 model = glm::mat4(1);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

constexpr float Z = 0.0f;
constexpr float UPFLOOR = 0.4f;
constexpr float DOWNFLOOR = -UPFLOOR;
constexpr float RIGHTFLOOR = 0.8f;
constexpr float LEFTFLOOR = -RIGHTFLOOR;

constexpr float FLOORCOLOR = 0.17f;
constexpr float WALLCOLOR = 0.30f;
constexpr float PLAYERCOLORR = 0.30f;
constexpr float PLAYERCOLORG = 0.30f;
constexpr float PLAYERCOLORB = 0.0f;

constexpr float UPWALL = 0.35f;
constexpr float DOWNWALL = -UPWALL;
constexpr float RIGHTWALL = 0.75f;
constexpr float LEFTWALL = -RIGHTWALL;
constexpr float WALLSIZE = 0.02f;
constexpr float PLAYERSIZE = 0.03f;

glm::vec3 playerPos;

std::vector<float> upObstacles;
std::vector<float> downObstacles;
std::vector<float> leftObstacles;
std::vector<float> rightObstacles;

std::random_device rand_dev;
std::mt19937 gen(rand_dev());

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

bool isFree(glm::vec3 p, float d)
{
    for (int i = 0; i < upObstacles.size(); ++i)
        if (p.y + d <= upObstacles[i] && p.y + d >= downObstacles[i])
            return false;
        else if (p.y - d <= upObstacles[i] && p.y + d >= downObstacles[i])
            return false;

    for (int i = 0; i < rightObstacles.size(); ++i)
        if (p.x + d <= rightObstacles[i] && p.x + d >= leftObstacles[i])
            return false;
        else if (p.x + d <= rightObstacles[i] && p.x - d >= leftObstacles[i])
            return false;

    std::cout << p.x << " " << p.y << " " << p.z << std::endl;
    return true;
}

std::vector<float> generateObstacles(int n)
{
    std::vector<float> obstacles;
    for (int i = 0; i < n; i++)
    {
        auto p = pos(WALLSIZE);
        float d = WALLSIZE;

        auto c = color();

        while (!isFree(p, d))
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
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Triangle", NULL, NULL);
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

    auto obstacles = generateObstacles(5);
    float obstacleArray[obstacles.size()];

    for (int i = 0; i < obstacles.size(); i++)
        obstacleArray[i] = obstacles[i];

    auto p = pos(PLAYERSIZE);
    while (!isFree(p, PLAYERSIZE))
        p = pos(PLAYERSIZE);

    playerPos = p;

    std::cout << "Player position: " << p.x << ", " << p.y << std::endl;

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

    glm::vec3 tempPos = glm::vec3(0.0f, 0.0f, 0.0f);

    float speed = static_cast<float>(0.5 * deltaTime);
    bool isMoving = true;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        tempPos = speed * glm::vec3(0.0f, 1.0f, 0.0f);
        isMoving = true;

        for (int i = 0; i < upObstacles.size(); ++i)
            if ((playerPos + tempPos).y + PLAYERSIZE >= downObstacles[i] && (playerPos + tempPos).y + PLAYERSIZE <= upObstacles[i] && playerPos.x + PLAYERSIZE >= leftObstacles[i] && playerPos.x - PLAYERSIZE <= rightObstacles[i])
            {
                isMoving = false;
                break;
            }

        if (isMoving)
            playerPos += tempPos, model = glm::translate(model, tempPos);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        tempPos = speed * glm::vec3(0.0f, -1.0f, Z);
        isMoving = true;


        for (int i = 0; i < downObstacles.size(); ++i)
            if ((playerPos + tempPos).y - PLAYERSIZE <= upObstacles[i] && (playerPos + tempPos).y + PLAYERSIZE >= downObstacles[i] && playerPos.x + PLAYERSIZE >= leftObstacles[i] && playerPos.x - PLAYERSIZE <= rightObstacles[i] )
            {
                isMoving = false;
                break;
            }

        if (isMoving)
            playerPos += tempPos, model = glm::translate(model, tempPos);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        tempPos = speed * glm::vec3(-1.0f, 0.0f, Z);
        isMoving = true;

        for (int i = 0; i < leftObstacles.size(); ++i)
            if ((playerPos + tempPos).x - PLAYERSIZE <= rightObstacles[i] && (playerPos + tempPos).x - PLAYERSIZE >= leftObstacles[i] && playerPos.y + PLAYERSIZE >= downObstacles[i] && playerPos.y - PLAYERSIZE <= upObstacles[i])
            {
                isMoving = false;
                break;
            }

        if (isMoving)
            playerPos += tempPos, model = glm::translate(model, tempPos);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        tempPos = speed * glm::vec3(1.0f, 0.0f, Z);
        isMoving = true;

        for (int i = 0; i < rightObstacles.size(); ++i)
            if ((playerPos + tempPos).x + PLAYERSIZE >= leftObstacles[i] && (playerPos + tempPos).x + PLAYERSIZE <= rightObstacles[i] && playerPos.y + PLAYERSIZE >= downObstacles[i] && playerPos.y - PLAYERSIZE <= upObstacles[i])
            {
                isMoving = false;
                break;
            }

        if (isMoving)
            playerPos += tempPos, model = glm::translate(model, tempPos);
    }
}
