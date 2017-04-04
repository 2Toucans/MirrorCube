#include "worldview.h"

void WorldView::render(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 surface) {
	// Begin the scene
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		// Setup the world, view, and projection matrices
		setupMatrices(pDevice);

		for (Mesh* m : meshes) {
			m->render(pDevice, surface);
		}

		// End the scene
		pDevice->EndScene();
	}
}

void WorldView::addMesh(Mesh* m) {
	meshes.push_back(m);
}

void WorldView::init(LPDIRECT3DDEVICE9 pDevice) {
	setupMatrices(pDevice);
}

void WorldView::setupMatrices(LPDIRECT3DDEVICE9 pDevice) {
	// For our world matrix, we will just leave it as the identity
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the 
	// origin, and define "up" to be in the y-direction.
	/*D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);*/
	D3DXMATRIXA16 matView;
	//D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	cam.getViewMatrix(&matView);
	pDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

Camera* WorldView::getCamera() {
	return &cam;
}