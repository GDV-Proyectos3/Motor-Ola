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
class Entidad;
class EntidadManager;
class Entity;
class OverlayManager;

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
	EntidadManager* _entidadManager = nullptr;
	OverlayManager* _overlayManager = nullptr;


	// Factoria Componentes

	//Physx
	//FMod
	//Cegui
	bool stop = false;
	//void handleEvents();

	int channel = 0;

	// Contador auxiliar
	int frame = 0;

public:
	Motor();
	~Motor();

	void initSystems();

	/// <summary>
	/// Añadir aquí todos los componentes que se hagan
	/// </summary>
	void registryComponents();

	/// <summary>
	/// Bucle principal de la ejecución del motor
	/// </summary>
	void mainLoop();

	/// <summary>
	/// Carga Juego.dll y llama a la funcion LoadGame()
	/// </summary>
	void loadDLLGame();

	/// <summary>
	/// Carga una escena del juego
	/// </summary>
	/// <param name="name">name es el archivo "scene.lua"</param>
	bool loadScene(std::string name);

	/// <summary>
	/// Funcion para hacer pruebas dentro del motor
	/// </summary>
	void loadTestMotorGame();

	// Getters and Setters
	void setStop(bool s);
	bool getStop();
	static void salir(Motor* m);
	static void deleteOverlay(Motor* m);
};

