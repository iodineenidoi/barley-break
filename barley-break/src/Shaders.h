#pragma once
#include <string>

unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath);
void setInt(unsigned int program, const char* name, int value);
void setVec2(unsigned int program, const char* name, float first, float second);