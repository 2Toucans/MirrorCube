#include "light.h"

Light::Light(LPDIRECT3DDEVICE9 pDevice): device(pDevice) {
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_AMBIENT, 0xFFFFFF);

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_POINT;
	light.Diffuse = D3DXCOLOR(0.75f, 0.25f, 0.25f, 1.0f);
	light.Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	light.Range = 50.0f;
	light.Attenuation0 = 0.0f;
	light.Attenuation1 = 0.1f;
	light.Attenuation2 = 0.0f;

	device->SetLight(0, &light);

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_SPOT;
	light.Diffuse = D3DXCOLOR(0.25f, 0.75f, 0.25f, 1.0f);
	light.Position = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	light.Range = 50.0f;
	light.Attenuation0 = 0.0f;
	light.Attenuation1 = 0.1f;
	light.Attenuation2 = 0.0f;
	light.Phi = D3DXToRadian(30.0f);
	light.Theta = D3DXToRadian(25.0f);
	light.Falloff = 1.0f;

	device->SetLight(1, &light);

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
	light.Direction = D3DXVECTOR3(-1.0f, -1.0f, 0);

	device->SetLight(2, &light);
}

void Light::toAmbient() {
	changeLight(0);
}

void Light::toPoint() {
	changeLight(1);
}

void Light::toSpotlight() {
	changeLight(2);
}

void Light::toDirectional() {
	changeLight(3);
}

void Light::changeLight(int lightIndex) {
	switch (lightIndex) {
	case 0: // ambient
		device->SetRenderState(D3DRS_AMBIENT, 0xFFFFFF);
		device->LightEnable(0, false);
		device->LightEnable(0, false);
		device->LightEnable(0, false);
		break;
	case 1: // point
		device->SetRenderState(D3DRS_AMBIENT, 0x000000);
		device->LightEnable(0, true);
		device->LightEnable(1, false);
		device->LightEnable(2, false);
		break;
	case 2: // spotlight
		device->SetRenderState(D3DRS_AMBIENT, 0x000000);
		device->LightEnable(0, false);
		device->LightEnable(1, true);
		device->LightEnable(2, false);
		break;
	case 3: // directional
		device->SetRenderState(D3DRS_AMBIENT, 0x000000);
		device->LightEnable(0, false);
		device->LightEnable(1, false);
		device->LightEnable(2, true);
		break;
	}
}