#pragma once
#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H
#include <functional>
class InputListener {
public:
	InputListener(std::function<void()> deligate);
	virtual void activate();
private:
	std::function<void()> onInput;
};
#endif