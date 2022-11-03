#pragma once
#include <functional>

void anim(float startValue, float endValue, float duration, std::function<void(float)> setter);
void processAnim(float deltaTime);
bool isInAnimation();