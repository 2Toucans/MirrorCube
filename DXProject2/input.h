#pragma once
#ifndef INPUT_H
#define INPUT_H
class Game;
#include <map>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include "camera.h"
#include "inputlistener.h"
#include "mouseinputlistener.h"
class Input {
public:
	Input();
	void regKey(int key, InputListener* i);
	void regMouseDown(MouseInputListener* i);
	void regMouseDrag(MouseInputListener* i);
	void regMouseUp(MouseInputListener* i);
	void regMouseWheel(MouseInputListener* i);

	void removeKey(int key, InputListener* i);
	void removeMouseDown(MouseInputListener* i);
	void removeMouseDrag(MouseInputListener* i);
	void removeMouseUp(MouseInputListener* i);
	void removeMouseWheel(MouseInputListener* i);

	int keypress(WPARAM key);
	int mousedrag(int xpos, int ypos);
	void mousePress(int xpos, int ypos);
	void mouseUnpress(int xpos, int ypos);
	int mouseWheel(float rot);
private:
	int lastMouseX;
	int lastMouseY;
	bool dragging;
	std::map<int, std::vector<InputListener*>> keyListeners;
	std::vector<MouseInputListener*> mouseDownListeners;
	std::vector<MouseInputListener*> mouseDragListeners;
	std::vector<MouseInputListener*> mouseUpListeners;
	std::vector<MouseInputListener*> mouseWheelListeners;
};
#endif