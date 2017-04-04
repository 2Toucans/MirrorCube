#include "gameobject.h"

void GameObject::move(int x2, int y2, int z2) {
	mesh->translate(x2 - x, y2 - y, z2 - z);
	x = x2;
	y = y2;
	z = x2;
}

void GameObject::rotate(float angle) {
	mesh->rotate(angle);
}