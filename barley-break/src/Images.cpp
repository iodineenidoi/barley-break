#include "Images.h"
#include <iostream>
#include <glad/glad.h>

#include "stb_image.h"

unsigned int loadTexture(const std::string& spritePath)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(spritePath.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cout << "Failed to load asset: " << spritePath << std::endl;
		return 0;
	}
	unsigned int texture = createTexture(data, width, height);
	stbi_image_free(data);
	return texture;
}

unsigned int createTexture(unsigned char* data, int width, int height)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	return texture;
}