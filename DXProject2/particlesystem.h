#pragma once
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include <d3dx9.h>
#include <vector>
#include <algorithm>
class ParticleSystem {
public:
	struct BoundingBox {
		D3DXVECTOR3 min;
		D3DXVECTOR3 max;
		bool isPointInside(D3DXVECTOR3& p) {
			if (p.x >= min.x && p.y >= min.y && p.z >= min.z &&
				p.x <= max.x && p.y <= max.y && p.z <= max.z)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};
	struct ParticleVertex {
		D3DXVECTOR3 position;
		D3DCOLOR color;
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	};
	struct Particle {
		D3DXVECTOR3 position;
		D3DXVECTOR3 velocity;
		D3DXVECTOR3 acceleration;
		float lifeTime;
		float age;
		D3DXCOLOR color;
		D3DXCOLOR colorFade;
		bool active;
	};

	ParticleSystem();
	virtual ~ParticleSystem();
	virtual void init(LPDIRECT3DDEVICE9 pDevice, LPCWSTR filename);
	virtual void reset();
	virtual void resetParticle(Particle* particle) = 0;
	virtual void addParticle();
	virtual void update(float timeDelta) = 0;

	virtual void preRender(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 surface);
	virtual void render(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 surface);
	virtual void postRender(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 surface);

	virtual void removeDeadParticles();
	D3DXVECTOR3 getRandomVector(D3DXVECTOR3* min, D3DXVECTOR3* max);

	bool isEmpty();
	bool isDead();
protected:
	DWORD fToDW(float f);
	float getRandomFloat(float min, float max);
	D3DXVECTOR3 origin;
	float emitRate;
	float size;
	IDirect3DTexture9* tex;
	IDirect3DVertexBuffer9* vBuff;
	std::vector<Particle> particles;
	int maxParticles;
	BoundingBox boundingBox;

	DWORD vbSize;
	DWORD vbOffset;
	DWORD vbBatchSize;

};
#endif