#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include <d3dx9.h>
class Camera
{
public:
	Camera();
	void strafe(float units); // left/right
	void fly(float units); // up/down
	void walk(float units); // forward/backward
	void pitch(float angle); // rotate on right vector
	void yaw(float angle); // rotate on up vector
	void roll(float angle); // rotate on look vector
	void getViewMatrix(D3DXMATRIX* V);
private:
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _pos;
};
#endif