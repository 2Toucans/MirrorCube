#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "mesh.h"
class GameObject {
public:
	GameObject(Mesh* m);
	void move(double x2, double y2, double z2);
	void rotate(float angle);
	void setClickRadius(double rad);
	double getClickRadius();
	double getX();
	double getY();
	double getZ();
private:
	double x, y, z;
	Mesh* mesh;
	double radius;
};
#endif