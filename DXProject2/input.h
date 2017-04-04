#pragma once
#ifndef INPUT_H
#define INPUT_H
class Game;
#include <Windows.h>
#include "camera.h"
#include "game.h"
class Input {
public:
	Input(Game* g);
	int keypress(WPARAM key);
	int mousedrag(int xpos, int ypos);
	void mousePress();
	void mouseUnpress();
	int mouseWheel(float rot);
private:
	int selectedMesh;
	int lastMouseX;
	int lastMouseY;
	bool dragging;
	Game* game;
};
#endif