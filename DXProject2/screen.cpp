#include "screen.h"
using namespace std;
Screen::Screen(HINSTANCE hInstance, int width, int height, int show) {
	HRESULT r = 0;//return values
	WNDCLASSEX wc;

	static TCHAR strAppName[] = TEXT("Aaron Freytag - Assignment 2");

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(Screen*);
	wc.lpfnWndProc = Screen::class_wndproc;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_HAND);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = strAppName;

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(NULL,
		strAppName,
		strAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width, height,
		NULL,
		NULL,
		hInstance,
		this);

	ShowWindow(hwnd, show);
	UpdateWindow(hwnd);

	dx = Direct3DCreate9(D3D_SDK_VERSION);//COM object
	if (dx == NULL) {
		free();
		throw "Could not create IDirect3D9 object";
	}

	r = initDirect3DDevice(hwnd, width, height, TRUE, D3DFMT_X8R8G8B8,dx, &device);
	if (FAILED(r)) {//FAILED is a macro that returns false if return value is a failure - safer than using value itself
		free();
		throw "Initialization of the device failed";
	}
}

Screen::~Screen() {
	free();
}

long CALLBACK Screen::wndproc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	POINT mousePoint;
	switch (uMessage) {
		case WM_CREATE:
		{
			return 0;
		}
		case WM_KEYDOWN:
		{
			if (input) {
				input->keypress(wParam);
			}
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			GetCursorPos(&mousePoint);
			input->mousePress(mousePoint.x, mousePoint.y);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			GetCursorPos(&mousePoint);
			input->mousedrag(mousePoint.x, mousePoint.y);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			GetCursorPos(&mousePoint);
			input->mouseUnpress(mousePoint.x, mousePoint.y);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			input->mouseWheel((float)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			return 0;
		}
		case WM_PAINT:
		{
			ValidateRect(hWnd, NULL);//basically saying - yeah we took care of any paint msg without any overhead
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
		}
	}
}

long CALLBACK Screen::class_wndproc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	if (uMessage == WM_CREATE) {
		// Window long pointer not yet initialized; need to set it
		SetWindowLong(hWnd, 0, (LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);
	}
	Screen* scr = (Screen*)GetWindowLong(hWnd, 0);
	if (scr) {
		return scr->wndproc(hWnd, uMessage, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

int Screen::initDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice) {
	D3DPRESENT_PARAMETERS d3dpp;//rendering info
	D3DDISPLAYMODE d3ddm;//current display mode info
	HRESULT r = 0;

	if (*ppDevice)
		(*ppDevice)->Release();

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	r = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if (FAILED(r)) {
		throw "Could not get display adapter information";
		return E_FAIL;
	}

	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = bWindowed ? d3ddm.Format : FullScreenFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//D3DSWAPEFFECT_COPY;
	d3dpp.hDeviceWindow = hWndTarget;
	d3dpp.Windowed = bWindowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = 0;//default refresh rate
	d3dpp.PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	r = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, ppDevice);
	if (FAILED(r)) {
		throw "Could not create the render device";
		return E_FAIL;
	}

	//	g_DeviceHeight = Height;
	//	g_DeviceWidth = Width;

	//	g_SavedPresParams = d3dpp;

	// Turn on the zbuffer
	device->SetRenderState(D3DRS_ZENABLE, TRUE);

	// Turn on ambient lighting 
	device->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	return S_OK;
}

void Screen::free() {
	if (device) {
		device->Release();
	}
	if (dx) {
		dx->Release();
	}
}

void Screen::render() {
	HRESULT r;
	if (!device) {
		throw "Cannot render because there is no device";
	}

	//clear the display arera with colour black, ignore stencil buffer
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 25), 1.0f, 0);

	//get pointer to backbuffer
	r = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backSurface);
	if (FAILED(r)) {
		throw "Couldn't get backbuffer";
	}

	for (Renderable* rendObj : renderables) {
		rendObj->render(device, backSurface);
	}

	backSurface->Release();//release lock
	backSurface = 0;
	device->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
}

void Screen::addRenderable(Renderable* r) {
	renderables.push_back(r);
}

void Screen::removeRenderable(Renderable* r) {
	renderables.erase(remove(renderables.begin(), renderables.end(), r), renderables.end());
}
LPDIRECT3DDEVICE9 Screen::getDevice() {
	return device;
}

void Screen::changeTitle(const wstring& newTitle) {
	SetWindowText(hwnd, newTitle.c_str());
}

void Screen::setInput(Input* i) {
	input = i;
}