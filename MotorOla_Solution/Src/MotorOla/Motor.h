#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>

class Motor
{
private:
	SDL_Window* window;
	const int WINWIDTH = 800;
	const int WINHEIGHT = 600;
	SDL_Renderer* renderer;
	//Ogre
	//Physx
	//FMod
	//Cegui
	bool stop = false;
	void handleEvents();

public:
	Motor();
	~Motor();
	void initSystems();
	void updateSystems();
};

