#include "SnowParticleSystem.h"

SnowParticleSystem::SnowParticleSystem(LPDIRECT3DDEVICE9 pDevice, BoundingBox* bounds, int numParticles) {
	size = 0.3f;
	vbSize = 2048;
	vbOffset = 0;
	vbBatchSize = 512;
	boundingBox = *bounds;
	for (int i = 0; i < numParticles; i++)
		addParticle();
	init(pDevice, L"snowflake.dds");
}

void SnowParticleSystem::resetParticle(Particle* p)
{
	p->active = true;
	// get random x, z coordinate for the position of the snowflake.
	p->position = getRandomVector(
		&(boundingBox.min),
		&(boundingBox.max));
	// no randomness for height (y-coordinate). Snowflake
	// always starts at the top of bounding box.
	p->position.y = boundingBox.max.y;
	// snowflakes fall downward and slightly to the left
	p->velocity.x = getRandomFloat(0.0f, 1.0f)*-3.0f;
	p->velocity.y = getRandomFloat(0.0f, 1.0f)*-10.0f;
	p->velocity.z = 0.0f;
	// white snowflake
	p->color = D3DCOLOR_XRGB(255, 255, 255);
}

void SnowParticleSystem::update(float timeDelta)
{
	for (auto& i : particles)
	{
		i.position += i.velocity * timeDelta;
		// is the point outside bounds?
		if (boundingBox.isPointInside(i.position) == false)
		{
			// nope so kill it, but we want to recycle dead
			// particles, so respawn it instead.
			resetParticle(&i);
		}
	}
}