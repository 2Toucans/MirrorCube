#pragma once
#ifndef SCREEN_H
#define SCREEN_H
/*
Screen

A DirectX window used to display various surfaces
*/
class Input;
#include <vector>
#include <Windows.h>
#include <Windowsx.h>
#include <algorithm>
#include <string>
#include <d3dx9core.h>
#include <d3d10.h>
#include <d3d9.h>
#include <d3dx9tex.h>
#include "renderable.h"
#include "input.h"
class Screen {
public:
	/*
	Screen(HWND hwnd)
	Creates a DirectX window
	*/
	Screen(HINSTANCE hwnd, int width, int height, int show);
	/*
	~Screen()
	Unprepares the screen, releasing its devices
	*/
	~Screen();
	static long CALLBACK class_wndproc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	/*
	void render()
	Re-renders the window by displaying to the back surface and switching the back surface with the front surface.
	*/
	void render();
	void addRenderable(Renderable* r);
	void removeRenderable(Renderable* r);
	void changeTitle(const std::wstring& newTitle);
	void setInput(Input* i);
	LPDIRECT3DDEVICE9 getDevice();
private:
	/*
	LPDIRECT3D9 dx
	The DirectX object
	*/
	LPDIRECT3D9 dx;
	/*
	LPDIRECT3DDEVICE9 device
	The Direct3D device object
	*/
	LPDIRECT3DDEVICE9 device;
	/*
	LPDIRECT3DSURFACE9 backSurface
	The back surface to render to
	*/
	LPDIRECT3DSURFACE9 backSurface;
	/*
	int initDirect3DDevice
	Initializes the Direct3D device based on the settings passed
	*/
	int initDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice);
	/*
	long CALLBACK wndproc()
	The wndproc used by the Win32 api
	*/
	virtual long CALLBACK wndproc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	/*
	void free()
	Frees the resources used by the screen
	*/
	void free();
	std::vector<Renderable*> renderables;
	HWND hwnd;
	Input* input;
};
#endif