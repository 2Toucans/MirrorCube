#include "input.h"
Input::Input(Game* g) : game(g), selectedMesh(0) {
}

int Input::keypress(WPARAM key) {
	switch (key) {
		case 0x57: //w
			game->getCamera()->walk(0.04);
			break;
		case 0x41: //a
			game->getCamera()->strafe(-0.04);
			break;
		case 0x53: //s
			game->getCamera()->walk(-0.04);
			break;
		case 0x44: //d
			game->getCamera()->strafe(0.04);
			break;
		case VK_SPACE:
			game->getCamera()->fly(0.04);
			break;
		case VK_SHIFT:
			game->getCamera()->fly(-0.04);
			break;
		case 0x31: //1
			selectedMesh = 0;
			break;
		case 0x32: //2
			selectedMesh = 1;
			break;
		case 0x33: //3
			game->getLight()->changeLight(0);
			break;
		case 0x34: //4
			game->getLight()->changeLight(1);
			break;
		case 0x35: //5
			game->getLight()->changeLight(2);
			break;
		case 0x36: //6
			game->getLight()->changeLight(3);
			break;
		case VK_LEFT:
			game->getCamera()->yaw(-0.04);
			break;
		case VK_RIGHT:
			game->getCamera()->yaw(0.04);
			break;
		case VK_UP:
			game->getCamera()->pitch(-0.04);
			break;
		case VK_DOWN:
			game->getCamera()->pitch(0.04);
			break;
		case VK_OEM_COMMA:
			game->getMesh(selectedMesh)->rotate(-0.04);
			break;
		case VK_OEM_PERIOD:
			game->getMesh(selectedMesh)->rotate(0.04);
			break;
	}
	return TRUE;
}

int Input::mousedrag(int xpos, int ypos) {
	if (dragging) {
		int xdiff = xpos - lastMouseX;
		int ydiff = ypos - lastMouseY;
		game->getMesh(selectedMesh)->translate(xdiff / 500.0, 0, -ydiff / 500.0);
	}
	lastMouseX = xpos;
	lastMouseY = ypos;
	return TRUE;
}

int Input::mouseWheel(float rot) {
	game->getMesh(selectedMesh)->translate(0.0, rot / 50.0, 0.0);
	return TRUE;
}

void Input::mousePress() {
	dragging = true;
}

void Input::mouseUnpress() {
	dragging = false;
}