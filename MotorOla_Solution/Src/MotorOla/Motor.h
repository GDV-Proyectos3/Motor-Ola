#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <iostream>

// Declaracion de clases necesarias
class OgreManager;
class InputManager;
class LoadResources;


class Motor
{
private:
	/*SDL_Window* window;
	const int WINWIDTH = 800;
	const int WINHEIGHT = 600;
	SDL_Renderer* renderer;*/

	// Managers
	OgreManager* _ogreManager = nullptr;
	InputManager* _inputManager = nullptr;
	LoadResources* _loadResources = nullptr;
	

	//Physx
	//FMod
	//Cegui
	bool stop = false;
	//void handleEvents();

public:
	Motor();
	~Motor();
	void initSystems();
	void updateSystems();
	void setStop(bool s);
	bool getStop();
};

