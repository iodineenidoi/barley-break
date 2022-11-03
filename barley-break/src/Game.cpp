#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Animation.h"
#include "Images.h"
#include "Input.h"
#include "Shaders.h"
#include "stb_image.h"

const int WINDOW_SIZE = 512;
GLFWwindow* window;

unsigned int VAO, VBO, EBO;
unsigned int platesTexture;
unsigned int shaderProgram;

float lastTime = 0.0f;
float deltaTime = 0.0f;

float vertices[] =
{
	-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
	 1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-1.0f,  1.0f, 0.0f,   0.0f, 1.0f, // top left
	 1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
};

unsigned int indices[] =
{
	0, 2, 3,
	0, 3, 1,
};

class plate
{
public:
	bool isEmpty;
	float xPos;
	float yPos;
	int texInRowIndex;
	int texInColumnIndex;
	int texturesInRow;
	int texturesInColumn;

	plate(float x, float y)
	{
		isEmpty = true;
		xPos = x;
		yPos = y;
		texInRowIndex = 0;
		texInColumnIndex = 0;
		texturesInRow = 0;
		texturesInColumn = 0;
	}

	plate(float x, float y, int xIndex, int yIndex, int xCount, int yCount)
	{
		isEmpty = false;
		xPos = x;
		yPos = y;
		texInRowIndex = xIndex;
		texInColumnIndex = yIndex;
		texturesInRow = xCount;
		texturesInColumn = yCount;
	}

	void setupShader(unsigned int shaderProgram, unsigned int texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUseProgram(shaderProgram);
		setVec2(shaderProgram, "u_rect.resolution", WINDOW_SIZE, WINDOW_SIZE);
		setVec2(shaderProgram, "u_rect.size", 128.0f, 128.0f);
		setVec2(shaderProgram, "u_rect.pos", xPos, yPos);

		setInt(shaderProgram, "u_imageSplitter.texRowIndex", texInRowIndex);
		setInt(shaderProgram, "u_imageSplitter.texColumnIndex", texInColumnIndex);
		setInt(shaderProgram, "u_imageSplitter.texturesInRow", texturesInRow);
		setInt(shaderProgram, "u_imageSplitter.texturesInColumn", texturesInColumn);
	}
};

plate plates[] =
{
	plate(-192.0f,  192.0f,  0, 3,  4, 4), // 1
	plate(-64.0f,   192.0f,  1, 3,  4, 4), // 2
	plate( 64.0f,   192.0f,  2, 3,  4, 4), // 3
	plate( 192.0f,  192.0f,  3, 3,  4, 4), // 4
	plate(-192.0f,  64.0f,   0, 2,  4, 4), // 5
	plate(-64.0f,   64.0f,   1, 2,  4, 4), // 6
	plate( 64.0f,   64.0f,   2, 2,  4, 4), // 7
	plate( 192.0f,  64.0f,   3, 2,  4, 4), // 8
	plate(-192.0f, -64.0f,   0, 1,  4, 4), // 9
	plate(-64.0f,  -64.0f,   1, 1,  4, 4), // 10
	plate( 64.0f,  -64.0f,   2, 1,  4, 4), // 11
	plate( 192.0f, -64.0f,   3, 1,  4, 4), // 12
	plate(-192.0f, -192.0f,  0, 0,  4, 4), // 13
	plate(-64.0f,  -192.0f,  1, 0,  4, 4), // 14
	plate( 64.0f,  -192.0f,  2, 0,  4, 4), // 15
	plate( 192.0f, -192.0f)
};

int getEmptyIndex()
{
	for (int i = 0; i < 16; i++)
	{
		if (plates[i].isEmpty)
			return i;
	}

	return -1;
}

void swapPlates(int index1, int index2)
{
	std::swap(plates[index1], plates[index2]);
	std::swap(plates[index1].xPos, plates[index2].xPos);
	std::swap(plates[index1].yPos, plates[index2].yPos);
}



void movePlate(int from, int to, bool isAxisX)
{
	if (isAxisX)
	{
		anim(plates[from].xPos, plates[to].xPos, 0.2f, [to](float value) { plates[to].xPos = value; });
	}
	else
	{
		anim(plates[from].yPos, plates[to].yPos, 0.2f, [to](float value) { plates[to].yPos = value; });
	}

	plates[to].xPos = plates[from].xPos;
	plates[to].yPos = plates[from].yPos;
	std::swap(plates[from], plates[to]);
}

bool moveUp(bool withAnimation)
{
	int emptyIndex = getEmptyIndex();
	switch (emptyIndex)
	{
	case 12:
	case 13:
	case 14:
	case 15:
		return false;
	}

	int swapIndex = emptyIndex + 4;
	if (withAnimation)
	{
		movePlate(swapIndex, emptyIndex, false);
	}
	else
	{
		swapPlates(swapIndex, emptyIndex);
	}

	return true;
}

bool moveDown(bool withAnimation)
{
	int emptyIndex = getEmptyIndex();
	switch (emptyIndex)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		return false;
	}

	int swapIndex = emptyIndex - 4;
	if (withAnimation)
	{
		movePlate(swapIndex, emptyIndex, false);
	}
	else
	{
		swapPlates(swapIndex, emptyIndex);
	}

	return true;
}

bool moveRight(bool withAnimation)
{
	int emptyIndex = getEmptyIndex();
	switch (emptyIndex)
	{
	case 0:
	case 4:
	case 8:
	case 12:
		return false;
	}

	int swapIndex = emptyIndex - 1;
	if (withAnimation)
	{
		movePlate(swapIndex, emptyIndex, true);
	}
	else
	{
		swapPlates(swapIndex, emptyIndex);
	}

	return true;
}

bool moveLeft(bool withAnimation)
{
	int emptyIndex = getEmptyIndex();
	switch (emptyIndex)
	{
	case 3:
	case 7:
	case 11:
	case 15:
		return false;
	}

	int swapIndex = emptyIndex + 1;
	if (withAnimation)
	{
		movePlate(swapIndex, emptyIndex, true);
	}
	else
	{
		swapPlates(swapIndex, emptyIndex);
	}
	return true;
}

void randomizePlates()
{
	std::srand(std::time(nullptr));
	for (int i = 0; i < 10000; i++)
	{
		int random = rand() % 4;
		switch (random)
		{
		case 0:
			moveLeft(false);
			break;
		case 1:
			moveRight(false);
			break;
		case 2:
			moveUp(false);
			break;
		case 3:
			moveDown(false);
			break;
		}
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);
}

void setupWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Barley Break", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeLimits(window, WINDOW_SIZE, WINDOW_SIZE, WINDOW_SIZE, WINDOW_SIZE);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		glfwTerminate();
		return;
	}

	glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void loadFiles()
{
	stbi_set_flip_vertically_on_load(true);

	platesTexture = loadTexture("src/assets/number-plates.png");
	shaderProgram = createShaderProgram("src/shaders/default.vert", "src/shaders/default.frag");
	if (platesTexture == 0 || shaderProgram == 0)
	{
		return;
	}

	glUseProgram(shaderProgram);
	setInt(shaderProgram, "u_mainTex", 0);
}

void createRenderObject()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void startGame()
{
	subscribeInput(GLFW_KEY_ESCAPE, []() {glfwSetWindowShouldClose(window, true); });
	subscribeInput(GLFW_KEY_UP, []() { moveUp(true); });
	subscribeInput(GLFW_KEY_DOWN, []() { moveDown(true); });
	subscribeInput(GLFW_KEY_RIGHT, []() { moveRight(true); });
	subscribeInput(GLFW_KEY_LEFT, []() { moveLeft(true); });
	subscribeInput(GLFW_KEY_SPACE, randomizePlates);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		if (isInAnimation())
		{
			processAnim(deltaTime);
		}
		else
		{
			processInput(window);
		}

		glClearColor(0.5f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		for (auto& plate : plates)
		{
			if (plate.isEmpty)
				continue;

			plate.setupShader(shaderProgram, platesTexture);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

void clearData()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
}
