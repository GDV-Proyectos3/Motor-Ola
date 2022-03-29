#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>
#include <iostream>

// Declaracion de clases necesarias
class OgreManager;
class InputManager;
class LoadResources;
class FMODAudioManager;
class PhysxManager;
class Entidad;
class Motor
{
private:
	/*SDL_Window* window;
	const int WINWIDTH = 800;
	const int WINHEIGHT = 600;
	SDL_Renderer* renderer;*/

	//LPCWSTR gameDLLRoute = L"..\\GameToLoad\\Juego"; // typedef const wchar_t* LPCWSTR, L"..." para indicar que se trata de un long char

	// Managers
	OgreManager* _ogreManager = nullptr;
	InputManager* _inputManager = nullptr;
	LoadResources* _loadResources = nullptr;
	FMODAudioManager* _audioManager = nullptr;
	PhysxManager* _physxManager = nullptr;
	

	// Factoria Componentes

	//Physx
	//FMod
	//Cegui
	bool stop = false;
	//void handleEvents();

	int channel = 0;

public:
	Motor();
	~Motor();

	void initSystems();

	void updateSystems();

	/// <summary>
	/// Carga Juego.dll y llama a la funcion LoadGame()
	/// </summary>
	void loadDLLGame();


	/// <summary>
	/// Funcion para hacer pruebas dentro del motor
	/// </summary>
	void loadTestMotorGame();

	// Getters and Setters
	void setStop(bool s);
	bool getStop();
};

