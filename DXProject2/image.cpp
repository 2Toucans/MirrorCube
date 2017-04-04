#include "image.h"
using namespace std;
Image::Image(const wstring& path, LPDIRECT3DDEVICE9 pDevice, int width, int height) {
	HRESULT r;
	LPDIRECT3DSURFACE9 tmpSurface;
	r = pDevice->CreateOffscreenPlainSurface(width, height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &surface, NULL);
	if (FAILED(r)) {
		throw "could not create surface";
	}

	r = loadBitmapToSurface(path,
		&tmpSurface, pDevice);
	if (FAILED(r)) {
		throw "could not load bitmap surface";
	}

	r = D3DXLoadSurfaceFromSurface(surface, NULL, NULL, tmpSurface, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
	if (FAILED(r)) {
		throw "did not copy surface";
	}
	
	tmpSurface->Release();
}

Image::~Image() {
	surface->Release();
}

void Image::render(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 backSurface) {
	HRESULT r;

	r = pDevice->UpdateSurface(surface, NULL, backSurface, NULL);
	if (FAILED(r)) {
		throw "did not copy surface";
	}
}

int Image::loadBitmapToSurface(wstring PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice) {
	HRESULT r;
	HBITMAP hBitmap;
	BITMAP Bitmap;

	hBitmap = (HBITMAP)LoadImage(NULL, PathName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap == NULL) {
		throw "Unable to load bitmap";
		return E_FAIL;
	}

	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	DeleteObject(hBitmap);//we only needed it for the header info to create a D3D surface

						  //create surface for bitmap
	r = pDevice->CreateOffscreenPlainSurface(Bitmap.bmWidth, Bitmap.bmHeight, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, ppSurface, NULL);

	if (FAILED(r)) {
		throw "Unable to create surface for bitmap load";
		return E_FAIL;
	}
	//load bitmap onto surface
	r = D3DXLoadSurfaceFromFile(*ppSurface, NULL, NULL, PathName.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(r)) {
		throw "Unable to load file to surface";
		return E_FAIL;
	}

	return S_OK;
}
