#include "particlesystem.h"
using namespace std;

ParticleSystem::ParticleSystem() {
}

void ParticleSystem::init(LPDIRECT3DDEVICE9 pDevice, LPCWSTR filename) {
	HRESULT hr = 0;
	hr = pDevice->CreateVertexBuffer(
		vbSize * sizeof(ParticleVertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		ParticleVertex::FVF,
		D3DPOOL_DEFAULT,
		&vBuff,
		0);
	if (FAILED(hr))
	{
		OutputDebugString(L"FU");
	}

	hr = D3DXCreateTextureFromFile(
		pDevice,
		filename,
		&tex);
	if (FAILED(hr))
	{
		OutputDebugString(L"FS");
	}
}

ParticleSystem::~ParticleSystem() {
}

void ParticleSystem::reset() {
	for (auto& i : particles) {
		resetParticle(&i);
	}
}

void ParticleSystem::addParticle() {
	Particle p;
	resetParticle(&p);
	particles.push_back(p);
}

void ParticleSystem::preRender(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 surface)
{
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	pDevice->SetRenderState(D3DRS_POINTSIZE, fToDW(size));
	pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, fToDW(0.0f));
	// control the size of the particle relative to distance
	pDevice->SetRenderState(D3DRS_POINTSCALE_A, fToDW(0.0f));
	pDevice->SetRenderState(D3DRS_POINTSCALE_B, fToDW(0.0f));
	pDevice->SetRenderState(D3DRS_POINTSCALE_C, fToDW(1.0f));
	// use alpha from texture
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void ParticleSystem::render(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 surface)
{
	if (!particles.empty())
	{
		// set render states
		preRender(pDevice, surface);
		pDevice->SetTexture(0, tex);
		pDevice->SetFVF(ParticleVertex::FVF);
		pDevice->SetStreamSource(0, vBuff, 0, sizeof(ParticleVertex));
		// start at beginning if we're at the end of the vb
		if (vbOffset >= vbSize)
			vbOffset = 0;
		ParticleVertex* v = 0;
		vBuff->Lock(
			vbOffset * sizeof(ParticleVertex),
			vbBatchSize * sizeof(ParticleVertex),
			(void**)&v,
			vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
		DWORD numParticlesInBatch = 0;
		//
		// Until all particles have been rendered.
		//
		for (auto& i : particles)
		{
			if (i.active)
			{
				//
				// Copy a batch of the living particles to the
				// next vertex buffer segment
				//
				v->position = i.position;
				v->color = (D3DCOLOR)i.color;
				v++; // next element;
				numParticlesInBatch++; //increase batch counter
									   // is this batch full?
				if (numParticlesInBatch == vbBatchSize)
				{
					//
					// Draw the last batch of particles that was
					// copied to the vertex buffer.
					//
					vBuff->Unlock();
					pDevice->DrawPrimitive(
						D3DPT_POINTLIST,
						vbOffset,
						vbBatchSize);
					//
					// While that batch is drawing, start filling the
					// next batch with particles.
					//
					// move the offset to the start of the next batch
					vbOffset += vbBatchSize;
					// don't offset into memory thats outside the vb's
					// range. If we're at the end, start at the beginning.
					if (vbOffset >= vbSize)
						vbOffset = 0;
					vBuff->Lock(
						vbOffset * sizeof(ParticleVertex),
						vbBatchSize * sizeof(ParticleVertex),
						(void**)&v,
						vbOffset ? D3DLOCK_NOOVERWRITE :
						D3DLOCK_DISCARD);
					numParticlesInBatch = 0; // reset for new batch
				}//end if
			}//end if
		}//end for
		vBuff->Unlock();
		// it’s possible that the LAST batch being filled never
		// got rendered because the condition
		// (numParticlesInBatch == _vbBatchSize) would not have
		// been satisfied. We draw the last partially filled batch now.
		if (numParticlesInBatch)
		{
			pDevice->DrawPrimitive(
				D3DPT_POINTLIST,
				vbOffset,
				numParticlesInBatch);
		}
		// next block
		vbOffset += vbBatchSize;
		postRender(pDevice, surface);
	}//end if
}// end render()

void ParticleSystem::postRender(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 surface)
{
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void ParticleSystem::removeDeadParticles()
{
	particles.erase(remove_if(particles.begin(), particles.end(), [](const Particle& p) {
		return !p.active;
	}), particles.end());
}

DWORD ParticleSystem::fToDW(float f) {
	return *((DWORD*)&f);
}

float ParticleSystem::getRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;
	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f;
	// return float in [lowBound, highBound] interval.
	return (f * (highBound - lowBound)) + lowBound;
}

bool ParticleSystem::isEmpty() {
	return particles.empty();
}

bool ParticleSystem::isDead() {
	for (auto& i : particles)
	{
		// is there at least one living particle?  If yes,
		// the system is not dead.
		if (i.active)
			return false;
	}
	// no living particles found, the system must be dead.
	return true;
}

D3DXVECTOR3 ParticleSystem::getRandomVector(D3DXVECTOR3* min, D3DXVECTOR3* max) {
	D3DXVECTOR3 out;
	out.x = getRandomFloat(min->x, max->x);
	out.y = getRandomFloat(min->y, max->y);
	out.z = getRandomFloat(min->z, max->z);
	return out;
}