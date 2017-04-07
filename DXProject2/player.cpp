#include "player.h"
#include <sstream>
using namespace std;

Player::Player() {
}

void Player::setInput(Input* i) {
	input = i;
}

void Player::setWorld(vector<GameObject*>* w) {
	world = w;
}

void Player::setCamera(Camera* cam) {
	view = cam;
}

void Player::setDevice(LPDIRECT3DDEVICE9 d) {
	device = d;
}

void Player::setupControls() {
	input->regKey(0x57, new InputListener([&v = view]() {v->walk(0.04); }));
	input->regKey(0x41, new InputListener([&v = view]() {v->strafe(-0.04); }));
	input->regKey(0x53, new InputListener([&v = view]() {v->walk(-0.04); }));
	input->regKey(0x44, new InputListener([&v = view]() {v->strafe(0.04); }));
	input->regKey(VK_SPACE, new InputListener([&v = view]() {v->fly(0.04); }));
	input->regKey(VK_SHIFT, new InputListener([&v = view]() {v->fly(-0.04); }));
	input->regKey(VK_LEFT, new InputListener([&v = view]() {v->yaw(-0.04); }));
	input->regKey(VK_RIGHT, new InputListener([&v = view]() {v->yaw(0.04); }));
	input->regKey(VK_UP, new InputListener([&v = view]() {v->pitch(-0.04); }));
	input->regKey(VK_DOWN, new InputListener([&v = view]() {v->pitch(0.04); }));
	input->regMouseDown(new MouseInputListener([this](int x, int y) {this->onClick(x, y);}));
}

double Player::pickObject(GameObject* o, double x, double y) {
	D3DXMATRIX trans;
	device->GetTransform(D3DTS_PROJECTION, &trans);
	float px = (2.0f * x - 1.0f) / trans(0, 0);
	float py = (2.0f * y - 1.0f) / trans(1, 1);
	D3DXVECTOR3 ray_origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ray_direction = D3DXVECTOR3(px, py, 1.0f);

	device->GetTransform(D3DTS_VIEW, &trans);
	//view->getViewMatrix(&trans);
	D3DXVec3TransformCoord(&ray_origin, &ray_origin, &trans);
	D3DXVec3TransformNormal(&ray_direction, &ray_direction, &trans);
	D3DXVec3Normalize(&ray_direction, &ray_direction);

	D3DXVECTOR3 obj_point = D3DXVECTOR3(o->getX(), o->getY(), o->getZ());
	D3DXVECTOR3 ray_obj = ray_origin - obj_point;
	double b = 2 * D3DXVec3Dot(&ray_direction, &ray_obj);
	double c = -2 * D3DXVec3Dot(&ray_origin, &obj_point) + D3DXVec3Dot(&obj_point, &obj_point) - o->getClickRadius() * o->getClickRadius();

	double disc = b * b - 4 * c;	// a is 1

	wostringstream debug = wostringstream();
	debug << x << "," << y << endl;

	OutputDebugString((LPWSTR)debug.str().c_str());

	if (disc < 0) {
		return -1; // sphere was not clicked on
	}

	double t0 = (-b + sqrt(float(disc))) / 2;
	double t1 = (-b - sqrt(float(disc))) / 2;

	if (t0 > 0 || t1 > 0) {
		return t1 < t0 ? t1 : t0;
	}

	return -1;
}

void Player::onClick(double x, double y) {
	input->removeMouseDrag(mouseDrag);
	input->removeMouseWheel(mouseWheel);
	GameObject* selectedObject = 0;
	LPDIRECT3DSURFACE9 surface;
	D3DDEVICE_CREATION_PARAMETERS params;
	RECT rect;
	device->GetCreationParameters(&params);
	GetWindowRect(params.hFocusWindow, &rect);
	double minVal = 999999;
	for (GameObject* o : *world) {
		double tmp = pickObject(o, (double)x / rect.right, (double)y / rect.bottom);
		if (tmp < minVal && tmp >= 0) {
			tmp = minVal;
			selectedObject = o;
		}
	}
	if (selectedObject != 0) {
		mouseDrag = new MouseInputListener([&o = selectedObject](int x, int y) {o->move(x, y, 0); });
		mouseWheel = new MouseInputListener([&o = selectedObject](int x, int y) {o->move(0, 0, x); });
		input->regMouseDrag(mouseDrag);
		input->regMouseWheel(mouseWheel);
	}
}