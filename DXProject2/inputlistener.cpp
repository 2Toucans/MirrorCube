#include "inputlistener.h"

InputListener::InputListener(std::function<void()> deligate) {
	onInput = deligate;
}

void InputListener::activate() {
	onInput();
}