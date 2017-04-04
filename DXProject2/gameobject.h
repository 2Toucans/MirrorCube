#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "mesh.h"
class GameObject {
public:
	void move(int x2, int y2, int z2);
	void rotate(float angle);
private:
	int x, y, z;
	Mesh* mesh;
};
#endif