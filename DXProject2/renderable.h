#pragma once
#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <d3dx9.h>
class Renderable {
public:
	virtual void render(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 surface) = 0;
};
#endif