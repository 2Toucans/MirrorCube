#include "gameobject.h"

GameObject::GameObject(Mesh* m) {
	mesh = m;
	D3DXVECTOR3* graphicsBuffer;
	D3DXVECTOR3 center;
	float r;
	m->getMesh()->LockVertexBuffer(0, (LPVOID*)&graphicsBuffer);
	D3DXComputeBoundingSphere(graphicsBuffer, m->getMesh()->GetNumVertices(), D3DXGetFVFVertexSize(m->getMesh()->GetFVF()), &center, &r);
	m->getMesh()->UnlockVertexBuffer();
	mesh->translate(-center.x, -center.y, -center.z);
	radius = r / 2;
}

void GameObject::move(double x2, double y2, double z2) {
	mesh->translate(x2, y2, z2);
	x += x2;
	y += y2;
	z += z2;
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