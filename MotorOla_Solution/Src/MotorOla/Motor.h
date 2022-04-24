#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>
#include <iostream>
#include <Ogre.h>

// Declaracion de clases necesarias
class OgreManager;
class InputManager;
class LoadResources;
class FMODAudioManager;
class PhysxManager;
class Entidad;
class EntidadManager;
class Entity;
class OverlayManager;

class Motor
{
private:
	//LPCWSTR gameDLLRoute = L"..\\GameToLoad\\Juego"; // typedef const wchar_t* LPCWSTR, L"..." para indicar que se trata de un long char

	bool stop = false;

	int channel = 0;

	// Contador auxiliar
	int frame = 0;

public:
	Motor();
	~Motor();

	void initSystems();

	/// <summary>
	/// A�adir aqu� todos los componentes que se hagan
	/// </summary>
	void registryComponents();

	/// <summary>
	/// Bucle principal de la ejecuci�n del motor
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
	bool loadMainMenu(std::string name,const char* get);

	/// <summary>
	/// Funcion para hacer pruebas dentro del motor
	/// </summary>
	void loadTestMotorGame();

	/// <summary>
	/// Mini-juego de prueba del motor
	/// </summary>
	//void loadPong();

	// Getters and Setters
	void setStop(bool s);
	bool getStop();


	static void salir(Motor* m);
	static void newGame(Motor* m);
	static void options(Motor* m);
	//static void deleteOverlay(Motor* m);
};

