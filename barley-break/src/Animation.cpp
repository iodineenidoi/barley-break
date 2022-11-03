#include "Animation.h"


float animDuration;
float animPassedTime;
float animStartValue, animEndValue;
bool isAnimationRunning = false;
std::function<void(float)> animSetter = nullptr;

void anim(float startValue, float endValue, float duration, std::function<void(float)> setter)
{
	animPassedTime = 0.0f;
	animDuration = duration;
	animStartValue = startValue;
	animEndValue = endValue;
	animSetter = setter;
	isAnimationRunning = true;
}

void processAnim(float deltaTime)
{
	animPassedTime += deltaTime;
	if (animPassedTime >= animDuration)
	{
		animSetter(animEndValue);
		isAnimationRunning = false;
	}
	else
	{
		float delta = (animEndValue - animStartValue) / animDuration;
		float currentValue = animStartValue + delta * animPassedTime;
		animSetter(currentValue);
	}
}

bool isInAnimation()
{
	return isAnimationRunning;
}
