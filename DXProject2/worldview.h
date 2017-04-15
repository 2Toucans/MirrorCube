#pragma once
#ifndef WORLDVIEW_H
#define WORLDVIEW_H
#include <vector>
#include <d3dx9.h>
#include "renderable.h"
#include "mesh.h"
#include "camera.h"
#include "particlesystem.h"
class WorldView: public Renderable {
public:
	virtual void render(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 surface);
	void init(LPDIRECT3DDEVICE9 pDevice);
	void addMesh(Mesh* m);
	void addParticleSystem(ParticleSystem* p);
	Camera* getCamera();
private:
	void setupMatrices(LPDIRECT3DDEVICE9 pDevice);
	std::vector<Mesh*> meshes;
	std::vector<ParticleSystem*> particleSystems;
	Camera cam;
};
#endif