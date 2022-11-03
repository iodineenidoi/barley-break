#pragma once
#include <functional>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window);
void subscribeInput(int key, const std::function<void()>& callback);