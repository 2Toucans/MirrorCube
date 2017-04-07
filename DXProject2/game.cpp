#include "game.h"
using namespace std;
Game::Game(HINSTANCE hInstance, int show) { // 640x480
	screen = new Screen(hInstance, 1920, 1080, show);
	QueryPerformanceFrequency(&queryFrequency);
	QueryPerformanceCounter(&lastFrameTime);
	currentTime = lastFrameTime;
	Image* img = new Image(L"baboon.bmp", screen->getDevice(), 1920, 1080);
	WorldView* wv = new WorldView();
	tiger = new Mesh(L"scannerarm.x", screen->getDevice());
	chair = new Mesh(L"chair.x", screen->getDevice());
	camera = wv->getCamera();
	wv->addMesh(tiger);
	wv->addMesh(chair);
	screen->addRenderable(img);
	screen->addRenderable(wv);
	Input* input = new Input();
	screen->setInput(input);
	light = new Light(screen->getDevice());
	input->regKey(0x33, new InputListener([l = this->light]() -> void {l->toAmbient(); }));
	input->regKey(0x34, new InputListener([l = this->light]() -> void {l->toPoint(); }));
	input->regKey(0x35, new InputListener([l = this->light]() -> void {l->toSpotlight(); }));
	input->regKey(0x36, new InputListener([l = this->light]() -> void {l->toDirectional(); }));
	world = new vector<GameObject*>;
	GameObject* gameTiger = new GameObject(tiger);
	GameObject* gameChair = new GameObject(chair);
	gameTiger->setClickRadius(1);
	gameChair->setClickRadius(1);
	world->push_back(gameTiger);
	world->push_back(gameChair);
	gameTiger->move(1.5, -3, 4);
	gameChair->move(-1.5, -3, 4);
	player = new Player;
	player->setInput(input);
	player->setWorld(world);
	player->setCamera(camera);
	player->setupControls();
	player->setDevice(screen->getDevice());
}

MSG Game::run() {
	return loop();
}

MSG Game::loop() {
	MSG msg;
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			tick();
		}
	}
	return msg;
}

void Game::tick() {
	screen->render();
	if (GetAsyncKeyState(VK_ESCAPE)) {
		PostQuitMessage(0);
	}
	updateFPS();
}

void Game::updateFPS() {
	QueryPerformanceCounter(&currentTime);
	++frames;
	if (currentTime.QuadPart - lastFrameTime.QuadPart >= queryFrequency.QuadPart) {
		fps = frames;
		frames = 0;
		lastFrameTime = currentTime;
	}
	wostringstream oss;
	oss << L"Aaron Freytag - Assignment 2 (FPS: " << fps << L")";
	screen->changeTitle(oss.str());
}

Screen* Game::getScreen() {
	return screen;
}

Mesh* Game::getMesh(int index) {
	if (index == 0) {
		return tiger;
	}
	else {
		return chair;
	}
}

Camera* Game::getCamera() {
	return camera;
}

Light* Game::getLight() {
	return light;
}