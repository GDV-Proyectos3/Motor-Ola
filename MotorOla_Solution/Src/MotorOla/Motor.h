#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

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

MOTOR_API class Motor
{
private:

	bool stop = false;

	int channel = 0;

	// Contador auxiliar
	int frame = 0;

	Entidad* cuboTest = nullptr;

public:
	MOTOR_API Motor();
	MOTOR_API ~Motor();

	MOTOR_API void initSystems();

	/// <summary>
	/// Añadir aqui todos los componentes que se hagan
	/// </summary>
	void registryComponents();

	/// <summary>
	/// Bucle principal de la ejecucion del motor
	/// </summary>
	MOTOR_API void mainLoop();

	/// <summary>
	/// Carga Juego.dll y llama a la funcion LoadGame()
	/// </summary>
	void loadDLLGame();

	/// <summary>
	/// Carga una escena del juego
	/// </summary>
	/// <param name="name">name es el archivo "scene.lua"</param>
	MOTOR_API bool loadScene(std::string name);
	MOTOR_API bool loadMenu(std::string name,const char* get);

	/// <summary>
	/// Funcion para hacer pruebas dentro del motor
	/// </summary>
	void loadTestMotorGame();



	// Getters and Setters
	MOTOR_API void setStop(bool s);
	bool getStop();
};

