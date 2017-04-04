#pragma once
#ifndef MESH_H
#define MESH_H
#include <string>
#include <d3dx9.h>
#include <atlbase.h>
class Mesh {
public:
	Mesh(const std::wstring& path, LPDIRECT3DDEVICE9 pDevice);
	~Mesh();
	void render(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 surface);
	void translate(float x, float y, float z);
	void rotate(float ang);
private:
	void load(const std::wstring& path, LPDIRECT3DDEVICE9 pDevice);
	LPD3DXMESH dxMesh;
	D3DMATERIAL9* materials;
	LPDIRECT3DTEXTURE9* textures;
	D3DXMATRIX worldMatrix;
	DWORD numOfMaterials;
};
#endif