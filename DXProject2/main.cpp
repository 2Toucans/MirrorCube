#include <Windows.h>
#include "screen.h"
#include "game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pstrCmdLine, int iCmdShow) {
	Game* g = new Game(hInstance, iCmdShow);
	MSG msg = g->run();

	delete g;
	return msg.wParam;
}