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
	D3DXMATRIX invTrans;
	D3DXVECTOR3 tmp_org;
	D3DXVECTOR3 tmp_dir;
	device->GetTransform(D3DTS_PROJECTION, &trans);
	float px = (2.0f * x - 1.0f) / trans(0, 0);
	float py = (-2.0f * y + 1.0f) / trans(1, 1);
	tmp_org = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	tmp_dir = D3DXVECTOR3(px, py, 1.0f);

	device->GetTransform(D3DTS_VIEW, &trans);
	D3DXMatrixInverse(&invTrans, NULL, &trans);

	D3DXVECTOR3 ray_origin = D3DXVECTOR3(
		tmp_org.x + invTrans(3, 0),
		tmp_org.y + invTrans(3, 1),
		tmp_org.z + invTrans(3, 2)
		);
	D3DXVECTOR3 ray_direction = D3DXVECTOR3(
		tmp_dir.x * invTrans(0, 0) + tmp_dir.y * invTrans(1, 0) + tmp_dir.z * invTrans(2, 0),
		tmp_dir.x * invTrans(0, 1) + tmp_dir.y * invTrans(1, 1) + tmp_dir.z * invTrans(2, 1),
		tmp_dir.x * invTrans(0, 2) + tmp_dir.y * invTrans(1, 2) + tmp_dir.z * invTrans(2, 2)
	);
	D3DXVec3Normalize(&ray_direction, &ray_direction);

	/*
	device->GetTransform(D3DTS_WORLD, &trans);
	D3DXMatrixInverse(&invTrans, NULL, &trans);

	D3DXVec3TransformCoord(&ray_origin, &ray_origin, &invTrans);
	D3DXVec3TransformNormal(&ray_direction, &ray_direction, &invTrans);
	*/

	D3DXVECTOR3 obj_point = D3DXVECTOR3(o->getX(), o->getY(), o->getZ());
	D3DXVECTOR3 ray_obj = ray_origin - obj_point;
	double b = 2 * D3DXVec3Dot(&ray_direction, &ray_obj);
	double c = D3DXVec3Dot(&ray_obj, &ray_obj) - o->getClickRadius() * o->getClickRadius();

	double disc = b * b - 4 * c;	// a is 1

	/*
	wostringstream debug = wostringstream();
	debug << "(" << ray_origin.x << ", " << ray_origin.y << ", " << ray_origin.z << ") "
		"(" << ray_direction.x << ", " << ray_direction.y << ", " << ray_direction.z << ") " 
		"(" << obj_point.x << ", " << obj_point.y << ", " << obj_point.z << ") "
		<< endl;

	debug << "End result: " << disc << endl;

	OutputDebugString((LPWSTR)debug.str().c_str());
	*/

	if (disc < 0) {
		return -1; // sphere was not clicked on
	}

	double t0 = (-b + sqrt((float)disc)) / 2;
	double t1 = (-b - sqrt((float)disc)) / 2;

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
		double tmp = pickObject(o, (double)x / (rect.right - rect.left), (double)y / (rect.bottom - rect.top));
		if (tmp < minVal && tmp >= 0) {
			minVal = tmp;
			selectedObject = o;
		}
	}
	if (selectedObject != 0) {
		mouseDrag = new MouseInputListener([o = selectedObject](int x, int y) {o->move((double)x/100, 0, -(double)y/80); });
		mouseWheel = new MouseInputListener([o = selectedObject](int x, int y) {o->move(0, (double)x / 1000, 0); });
		input->regMouseDrag(mouseDrag);
		input->regMouseWheel(mouseWheel);
	}
}