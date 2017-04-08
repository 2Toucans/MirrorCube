#include "mesh.h"
using namespace std;
Mesh::Mesh(const wstring& path, LPDIRECT3DDEVICE9 pDevice) {
	D3DXMatrixIdentity(&worldMatrix);
	load(path, pDevice);
}
Mesh::~Mesh() {
	if (materials != NULL)
		delete[] materials;

	if (textures)
	{
		for (DWORD i = 0; i < numOfMaterials; i++)
		{
			if (textures[i])
				textures[i]->Release();
		}
		delete[] textures;
	}
	if (dxMesh != NULL) {
		dxMesh->Release();
	}
}
void Mesh::render(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 surface) {
	pDevice->SetTransform(D3DTS_WORLD, &worldMatrix);
	for (DWORD i = 0; i<numOfMaterials; i++)
	{
		// Set the material and texture for this subset
		pDevice->SetMaterial(&materials[i]);
		pDevice->SetTexture(0, textures[i]);

		// Draw the mesh subset
		dxMesh->DrawSubset(i);
	}
}
void Mesh::load(const wstring& path, LPDIRECT3DDEVICE9 pDevice) {
	LPD3DXBUFFER pD3DXMtrlBuffer;

	// Load the mesh from the specified file
	if (FAILED(D3DXLoadMeshFromX(path.c_str(), D3DXMESH_SYSTEMMEM,
		pDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &numOfMaterials,
		&dxMesh)))
	{
		throw "Could not find file";
	}

	// We need to extract the material properties and texture names from the 
	// pD3DXMtrlBuffer
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	materials = new D3DMATERIAL9[numOfMaterials];
	textures = new LPDIRECT3DTEXTURE9[numOfMaterials];

	for (DWORD i = 0; i<numOfMaterials; i++)
	{
		// Copy the material
		materials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		materials[i].Ambient = materials[i].Diffuse;

		textures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(CA2CT(d3dxMaterials[i].pTextureFilename)) > 0)
		{
			// Create the texture
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				CA2CT(d3dxMaterials[i].pTextureFilename),
				&textures[i])))
			{
				// If texture is not in current folder, try parent folder
				const TCHAR* strPrefix = TEXT("..\\");
				const int lenPrefix = lstrlen(strPrefix);
				TCHAR strTexture[MAX_PATH];
				lstrcpyn(strTexture, strPrefix, MAX_PATH);
				lstrcpyn(strTexture + lenPrefix, CA2CT(d3dxMaterials[i].pTextureFilename), MAX_PATH - lenPrefix);
				// If texture is not in current folder, try parent folder
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					strTexture,
					&textures[i])))
				{
					throw "Could not find texture map";
				}
			}
		}
	}

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();
}

void Mesh::translate(float x, float y, float z) {
	D3DXMATRIX transmtx;
	D3DXMatrixTranslation(&transmtx, x, y, z);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &transmtx);
}

void Mesh::rotate(float ang) {
	D3DXMATRIX transmtx;
	/*float tx, ty, tz;
	tx = worldMatrix._41;
	ty = worldMatrix._42;
	tz = worldMatrix._43;*/
	D3DXMatrixRotationY(&transmtx, ang);
	D3DXMatrixMultiply(&worldMatrix, &transmtx, &worldMatrix);
}

LPD3DXMESH Mesh::getMesh() {
	return dxMesh;
}