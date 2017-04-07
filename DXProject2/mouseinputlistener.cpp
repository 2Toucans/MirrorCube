#include "mouseinputlistener.h"

MouseInputListener::MouseInputListener(std::function<void(int, int)> deligate) {
	onInput = deligate;
}

void MouseInputListener::activate(int x, int y) {
	onInput(x, y);
}

void MouseInputListener::activate() {
	onInput(0, 0);
}