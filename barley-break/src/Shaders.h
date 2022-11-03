#pragma once
#include <string>

unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath);
void setInt(unsigned int program, const char* name, int value);