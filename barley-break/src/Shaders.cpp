#include "Shaders.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>

std::string loadFileText(const char* path)
{
	std::string loadedText;
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		file.open(path);
		std::stringstream fileStream;
		fileStream << file.rdbuf();
		file.close();
		loadedText = fileStream.str();
	}
	catch (std::exception ex)
	{
		std::cout << "Error: can't load shader file." << std::endl << "File: " << path << std::endl << "Error: " << ex.what() << std::endl;
	}


	return loadedText;
}

unsigned int createShader(const std::string& shaderCode, const std::string& shaderType)
{
	int success;
	char logInfo[512];

	unsigned int shader = 0;
	if (shaderType == "vertex")
		shader = glCreateShader(GL_VERTEX_SHADER);
	if (shaderType == "fragment")
		shader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* code = shaderCode.c_str();
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, logInfo);
		std::cout << shaderType << " shader compile error. Info: " << logInfo << std::endl;
	}

	return shader;
}

unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode = loadFileText(vertexPath);
	std::string fragmentCode = loadFileText(fragmentPath);

	unsigned int vertexShader = createShader(vertexCode, "vertex");
	unsigned int fragmentShader = createShader(fragmentCode, "fragment");

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Linking shaders error: " << infoLog << std::endl << "Vertex path : " << vertexPath << std::endl << "Fragment path : " << fragmentPath << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void setInt(unsigned int program, const char* name, int value)
{
	glUniform1i(glGetUniformLocation(program, name), value);
}
