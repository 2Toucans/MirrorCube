#pragma once
#ifndef IMAGE_H
#define IMAGE_H
#include <d3dx9core.h>
#include <iostream>
#include "renderable.h"
/*
	Image
	A basic image stored on a surface to be displayed to another surface
*/
class Image: public Renderable {
public:
	/*
		Image(string p)
		Creates the image surface from a bitmap file retreived from the path p
	*/
	Image(const std::wstring& path, LPDIRECT3DDEVICE9 pDevice, int width, int height);
	/*
		getSurface()
		Getter for the surface of the image
	*/
	~Image();
	virtual void render(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 backSurface);
private:
	/*
		int loadBitmapToSurface(wstring PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice);
	*/
	int loadBitmapToSurface(std::wstring PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice);
	/*
		surface
		The surface the image will be stored on
	*/
	LPDIRECT3DSURFACE9 surface;
};
#endif