#pragma once
#include <d3dx9.h>
#include "renderable.h"
class Light {
public:
	Light(LPDIRECT3DDEVICE9 pDevice);
	void changeLight(int lightIndex);
private:
	D3DXCOLOR color;
	LPDIRECT3DDEVICE9 device;
};