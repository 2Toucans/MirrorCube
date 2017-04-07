#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <d3dx9.h>
#include "input.h"
#include "camera.h"
#include "gameobject.h"
class Player {
public:
	Player();
	void setInput(Input* i);
	void setWorld(std::vector<GameObject*>* w);
	void setCamera(Camera* cam);
	void setupControls();
	void setDevice(LPDIRECT3DDEVICE9 d);
	double pickObject(GameObject* o, double x, double y);
	void onClick(double x, double y);
private:
	Input* input;
	Camera* view;
	MouseInputListener* mouseDrag;
	MouseInputListener* mouseWheel;
	GameObject* selectedObject;
	LPDIRECT3DDEVICE9 device;
	std::vector<GameObject*>* world;
};
#endif