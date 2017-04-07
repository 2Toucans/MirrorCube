#pragma once
#ifndef GAME_H
#define GAME_H
/*
	Game
	The main loop of the program
	Re-renders the screen as quickly as possible
*/
#include <sstream>
#include <Windows.h>
#include "camera.h"
#include "image.h"
#include "worldview.h"
#include "mesh.h"
#include "input.h"
#include "screen.h"
#include "light.h"
#include "inputlistener.h"
#include "gameobject.h"
#include "player.h"

class Screen;
class Game {
public:
	/*
		virtual MSG start()
		Initializes the game and starts the game loop
	*/
	Game(HINSTANCE hInstance, int show);
	virtual MSG run();
	Screen* getScreen();
	Mesh* getMesh(int index);
	Camera* getCamera();
	Light* getLight();
private:
	/*
		virtual MSG loop()
		The game loop; exits if a quit message is posted, otherwise run a tick
	*/
	MSG loop();
	/*
		tick()
		Renders the screen, closing if a quit message is posted
	*/
	void tick();
	void updateFPS();
	Screen* screen;
	int frames;
	int fps;
	LARGE_INTEGER lastFrameTime;
	LARGE_INTEGER currentTime;
	LARGE_INTEGER queryFrequency;
	Mesh* tiger;
	Mesh* chair;
	Camera* camera;
	Light* light;
	Player* player;
	std::vector<GameObject*>* world;
};
#endif