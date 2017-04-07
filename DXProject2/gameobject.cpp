#include "gameobject.h"

GameObject::GameObject(Mesh* m) {
	mesh = m;
}

void GameObject::move(double x2, double y2, double z2) {
	mesh->translate(x2 - x, y2 - y, z2 - z);
	x = x2;
	y = y2;
	z = x2;
}

double GameObject::getX() {
	return x;
}

double GameObject::getY() {
	return y;
}

double GameObject::getZ() {
	return z;
}

void GameObject::setClickRadius(double r) {
	radius = r;
}

double GameObject::getClickRadius() {
	return radius;
}

void GameObject::rotate(float angle) {
	mesh->rotate(angle);
}