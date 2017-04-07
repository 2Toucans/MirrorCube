#include "input.h"
using namespace std;
Input::Input() {
}

void Input::regKey(int key, InputListener* i) {
	auto it = keyListeners.find(key);
	if (it == keyListeners.end()) {
		it = keyListeners.insert(make_pair(key, *(new vector<InputListener*>))).first;
	}
	it->second.push_back(i);
}

void Input::regMouseDown(MouseInputListener* i) {
	mouseDownListeners.push_back(i);
}

void Input::regMouseDrag(MouseInputListener* i) {
	mouseDragListeners.push_back(i);
}

void Input::regMouseUp(MouseInputListener* i) {
	mouseUpListeners.push_back(i);
}

void Input::regMouseWheel(MouseInputListener* i) {
	mouseWheelListeners.push_back(i);
}

void Input::removeKey(int key, InputListener* i) {
	auto it = keyListeners.find(key);
	if (it == keyListeners.end()) {
		vector<InputListener*> il = it->second;
		il.erase(remove(il.begin(), il.end(), i), il.end());
	}
}

void Input::removeMouseDown(MouseInputListener* i) {
	mouseDownListeners.erase(remove(mouseDownListeners.begin(), mouseDownListeners.end(), i), mouseDownListeners.end());
}

void Input::removeMouseDrag(MouseInputListener* i) {
	mouseDragListeners.erase(remove(mouseDragListeners.begin(), mouseDragListeners.end(), i), mouseDragListeners.end());
}

void Input::removeMouseUp(MouseInputListener* i) {
	mouseUpListeners.erase(remove(mouseUpListeners.begin(), mouseUpListeners.end(), i), mouseUpListeners.end());
}

void Input::removeMouseWheel(MouseInputListener* i) {
	mouseWheelListeners.erase(remove(mouseWheelListeners.begin(), mouseWheelListeners.end(), i), mouseWheelListeners.end());
}

int Input::keypress(WPARAM key) {
	/*
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
	*/
	if (keyListeners.find(key) != keyListeners.end()) {
		for (InputListener* i : keyListeners.find(key)->second) {
			i->activate();
		}
	}
	return TRUE;
}

int Input::mousedrag(int xpos, int ypos) {
	if (dragging) {
		int xdiff = xpos - lastMouseX;
		int ydiff = ypos - lastMouseY;
		//game->getMesh(selectedMesh)->translate(xdiff / 500.0, 0, -ydiff / 500.0);
		for (MouseInputListener* i : mouseDragListeners) {
			i->activate(xdiff, ydiff);
		}
	}
	lastMouseX = xpos;
	lastMouseY = ypos;
	return TRUE;
}

int Input::mouseWheel(float rot) {
	//game->getMesh(selectedMesh)->translate(0.0, rot / 50.0, 0.0);
	for (MouseInputListener* i : mouseWheelListeners) {
		i->activate((int)(rot * 100), 0);
	}
	return TRUE;
}

void Input::mousePress(int xpos, int ypos) {
	for (MouseInputListener* i : mouseDownListeners) {
		i->activate(xpos, ypos);
	}
	dragging = true;
}

void Input::mouseUnpress(int xpos, int ypos) {
	for (MouseInputListener* i : mouseUpListeners) {
		i->activate(xpos, ypos);
	}
	dragging = false;
}