#include "Input.h"

#include <map>
#include <set>

std::set<int> buttonsPressed;
std::map<int, std::function<void()>> callbacks;


bool isClicked(GLFWwindow* window, int key)
{
	if (glfwGetKey(window, key))
	{
		if (buttonsPressed.find(key) == buttonsPressed.end())
		{
			buttonsPressed.insert(key);
		}
		return false;
	}

	if (buttonsPressed.find(key) == buttonsPressed.end())
	{
		return false;
	}

	buttonsPressed.erase(key);
	return true;
}

void processInput(GLFWwindow* window)
{
	for (auto it = callbacks.begin(), end = callbacks.end(); it != end; ++it)
	{
		if (isClicked(window, it->first))
		{
			it->second();
		}
	}
}

void subscribeInput(int key, const std::function<void()>& callback)
{
	auto it = callbacks.find(key);
	if (it == callbacks.end())
	{
		callbacks.insert({ key, callback });
	}
	else
	{
		it->second = callback;
	}
}

