#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>

#include "Motor.h"
#include <Windows.h>
#include "LuaReader.h"

// Ogre
#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreVector.h>

// Managers
#include "utils/Singleton.h"
#include "OgreManager.h"
#include "EntidadManager.h"
#include "InputManager.h"
#include "LoadResources.h"
#include "FMODAudioManager.h"
#include "OverlayManager.h"
#include "PhysxManager.h"

// Componentes
#include "Entidad.h"
#include "Componente.h"
#include "ComponenteFactoria.h"
#include "ComponenteRegistro.h"
#include "Transform.h"
#include "Mesh.h"
#include "AudioSource.h"
#include "Collider.h"
#include "RigidBody.h"
#include <iostream>

typedef HRESULT(CALLBACK* LPFNDLLFUNC1)(DWORD, UINT*);
HINSTANCE hDLL;               // Handle to DLL

Motor::Motor()
{
	// Inicia los managers
	Singleton<LoadResources>::instance();
	Singleton<OgreManager>::instance();	
	Singleton<EntidadManager>::instance();
	Singleton<FMODAudioManager>::instance();
	Singleton<OverlayManager>::instance();
	Singleton<PhysxManager>::instance();
	std::cout << "MANAGERS INSTANCIADOS CORRECTAMENTE\n";
}

Motor::~Motor()
{
	FreeLibrary(hDLL);
	// Destruye los managers en orden inverso a la creaci�n (PC: puede que esto no sea necesario porque al cerrar se borran solos)
	if (Singleton<FMODAudioManager>::instance() != nullptr) delete Singleton<FMODAudioManager>::instance();
	if (Singleton<EntidadManager>::instance() != nullptr) delete Singleton<EntidadManager>::instance();
	if (Singleton<LoadResources>::instance() != nullptr) delete Singleton<LoadResources>::instance();
	
}

void Motor::initSystems()
{
	// Inicia los sistemas
	Singleton<LoadResources>::instance()->init();
	Singleton<OgreManager>::instance()->init();
	Singleton<FMODAudioManager>::instance()->init();
	Singleton<OverlayManager>::instance()->init(Singleton<OgreManager>::instance(),this);
	pm().init();

	// Se registran los componentes que conoce el motor
	registryComponents();

	std::cout << "ANTES DE CARGAR JUEGO TRY\n";

	// El motor intenta cargar un juego, pero si hay algun error se arranca con la funcion loadTestMotorGame
	try {
		loadDLLGame();
	}
	catch (const char* error) {
		std::cout << "Error: " << error << "\n";
		loadTestMotorGame();
	}
	std::cout << "DESPUES DE CARGAR JUEGO TRY\n";
}

void Motor::registryComponents()
{
	// Apuntar aqui todos los componentes del motor (apuntar solo despues de refactorizar)
	try {
		ComponenteRegistro::ComponenteRegistro<Transform>("transform");
		ComponenteRegistro::ComponenteRegistro<Mesh>("mesh");
		ComponenteRegistro::ComponenteRegistro<Collider>("collider");
		ComponenteRegistro::ComponenteRegistro<RigidBody>("rigidbody");
	}
	catch (const char* error) {
		std::cout << "Error registrando los componentes del motor: \n" << error << "\n";
	}
}

void Motor::mainLoop()
{
	std::cout << "------------------- COMIENZA EL BUCLE PRINCIPAL -------------------\n";
	//Actualiza el motor. Bucle input->update/fisicas->render
	SDL_Event event;
	std::cout << Singleton<EntidadManager>::instance() << "\n";

	while (!stop) {
		
		Singleton<EntidadManager>::instance()->refresh();

		// Recoger el Input
		ih().clearState();

		while (SDL_PollEvent(&event))
			ih().update(event);

		// Cierra la aplicacion con ESCAPE
		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
			stop = true;
			continue;
		}

		// Actualizar las fisicas de las entidades
		pm().runPhysX();

		Singleton<EntidadManager>::instance()->refresh();

		// Actualiza los transforms de las entitys despues de las fisicas
		if (Singleton<OverlayManager>::instance() != nullptr) {
			Singleton<OverlayManager>::instance()->update();
		}

		// Actualiza el resto de componentes (tambien los del juego)
		Singleton<EntidadManager>::instance()->update();

		// Renderiza las entidades
		Singleton<OgreManager>::instance()->update();
	}
}

void Motor::loadDLLGame()
{
	HINSTANCE hDLL;
	LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer
	HRESULT hrReturnVal;
	std::cout<<"Entra en loadDLL"<<std::endl;
#ifdef NDEBUG
	hDLL = LoadLibrary(L".\\Juego");	// typedef const wchar_t* LPCWSTR, L"..." para indicar que se trata de un long char
#else
	hDLL = LoadLibrary(L".\\Juego_d");	// typedef const wchar_t* LPCWSTR, L"..." para indicar que se trata de un long char
#endif

	if (NULL != hDLL)
	{
		lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL, "LoadGame");
		if (NULL != lpfnDllFunc1)
		{
			lpfnDllFunc1(NULL, NULL);
		}
		else throw "Function LoadGame not found in DLL";
	}
	else throw "DLL not found";
}

bool Motor::loadScene(std::string name) {
	try {
		// Borra las entidades de la escena actual
		Singleton<EntidadManager>::instance()->pauseEntidades();

		// Devuelve la ruta de la escena
		std::string sceneRoute = Singleton<LoadResources>::instance()->scene(name).c_str();

		// Lee la escena cargando todas las entidades y sus componentes
		readFile(sceneRoute);
	}
	catch (std::exception e) {
#if (defined _DEBUG)
		std::cerr << e.what();
#endif
		return false;
	}
	return true;
}
bool Motor::loadMenu(std::string name,const char*get) {
	try {
		// Borra las entidades de la escena actual
		Singleton<EntidadManager>::instance()->pauseEntidades();

		// Devuelve la ruta de la escena
		std::string sceneRoute = Singleton<LoadResources>::instance()->scene(name).c_str();

		// Lee la escena cargando todas las entidades y sus componentes
		readFileMenus(sceneRoute,get);
	}
	catch (std::exception e) {
#if (defined _DEBUG)
		std::cerr << e.what();
#endif
		return false;
	}
	return true;
}

void Motor::loadTestMotorGame() 
{
	loadScene("TestScene.lua");
}

bool Motor::getStop()
{
	return stop;
}



void Motor::setStop(bool s)
{
	stop = s;
}

