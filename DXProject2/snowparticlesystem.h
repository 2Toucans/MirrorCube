#pragma once
#ifndef SNOW_PARTICLE_SYSTEM_H
#define SNOW_PARTICLE_SYSTEM_H
#include "particlesystem.h"
class SnowParticleSystem: public ParticleSystem {
public:
	SnowParticleSystem(LPDIRECT3DDEVICE9 pDevice, BoundingBox* bounds, int numParticles);
	void resetParticle(Particle* attribute);
	void update(float timeDelta);
};
#endif