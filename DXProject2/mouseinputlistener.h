#pragma once
#ifndef MOUSE_INPUT_LISTENER_H
#define MOUSE_INPUT_LISTENER_H
#include "inputlistener.h"
#include <functional>
class MouseInputListener {
public:
	MouseInputListener(std::function<void(int, int)> deligate);
	virtual void activate();
	virtual void activate(int x, int y);

private:
	std::function<void(int, int)> onInput;
};
#endif