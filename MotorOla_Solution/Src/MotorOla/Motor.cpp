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
#include "Movible.h"
#include "AudioSource.h"
#include <iostream>

typedef HRESULT(CALLBACK* LPFNDLLFUNC1)(DWORD, UINT*);

Motor::Motor()
{
	// Inicia los managers (PC: ya que no se usan métodos, se podría dejar con init que para el caso es lo mismo)
	Singleton<LoadResources>::instance();
	Singleton<OgreManager>::instance();	
	Singleton<EntidadManager>::instance();
	Singleton<FMODAudioManager>::instance();
	Singleton<OverlayManager>::instance();
	Singleton<PhysxManager>::instance();



}

Motor::~Motor()
{
	// Destruye los managers en orden inverso a la creaci�n (PC: puede que esto no sea necesario porque al cerrar se borran solos)
	//if (Singleton<PhysxManager>::instance() != nullptr) delete Singleton<PhysxManager>::instance();
	if (Singleton<OverlayManager>::instance() != nullptr) delete Singleton<OverlayManager>::instance();
	if (Singleton<FMODAudioManager>::instance() != nullptr) delete Singleton<FMODAudioManager>::instance();
	if (Singleton<EntidadManager>::instance() != nullptr) delete Singleton<EntidadManager>::instance();
	if (Singleton<OgreManager>::instance() != nullptr) delete Singleton<OgreManager>::instance();
	if (Singleton<LoadResources>::instance() != nullptr) delete Singleton<LoadResources>::instance();

	//if (_overlayManager) delete _overlayManager;
	//SDL_QuitSubSystem(SDL_INIT_VIDEO);
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

	std::cout << "EM: " << Singleton<EntidadManager>::instance() << "\n";
	std::cout << "LR " << Singleton<LoadResources>::instance() << "\n";
	std::cout << "FMOD: " << Singleton<FMODAudioManager>::instance() << "\n";

	// El motor intenta cargar un juego, pero si hay algun error se arranca con la funcion loadTestMotorGame
	try {
		loadDLLGame();
	}
	catch (const char* error) {
		std::cout << "Error: " << error << "\n";
		loadTestMotorGame();
	}

	// Carga una escena con Lua
	//if(!loadScene("TestScene.lua"))
	if(!loadMainMenu("MainMenuScene.lua","GetMainMenu"))
		throw "Error loading a Scene\n";
}

void Motor::registryComponents()
{
	// Apuntar aqui todos los componentes del motor (apuntar solo despues de refactorizar)
	try {
		ComponenteRegistro::ComponenteRegistro<Transform>("transform");
		ComponenteRegistro::ComponenteRegistro<Mesh>("mesh");
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
		// Update PhysX: (PC: el 'pm()' igual que 'ih()' es un alias es decir una forma breve para nombrar la llamada a '(...)::instance()')
		pm().update(); 

		// Actualiza los transforms de las entitys despues de las fisicas
		if (Singleton<OverlayManager>::instance() != nullptr) {
			Singleton<OverlayManager>::instance()->update();
		}
		// Actualiza el resto de componentes (tambien los del juego)
		Singleton<EntidadManager>::instance()->update();

		// Renderiza las entidades
		Singleton<OgreManager>::instance()->update();

		// Contador de frames que los muestra cada 100 frames
#if (defined _DEBUG)
		if (++frame % 60 == 0) {
			//// Prueba de los prefabs
			//float x = rand() % 800 - 400;
			//float y = rand() % 600 - 300;
			//Entidad::instantiate("Bala.prefab", Vectola3D(x,y,0));
			EntidadManager* aux = Singleton<EntidadManager>::instance();
			//std::cout << aux << std::endl;
			std::cout << "FrameCount: " << frame << "\n";
		}
#endif
	}
}

void Motor::loadDLLGame()
{
	HINSTANCE hDLL;               // Handle to DLL
	LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer
	HRESULT hrReturnVal;
#ifdef _NDEBUG
	hDLL = LoadLibrary(L"..\\GameToLoad\\Juego");	// typedef const wchar_t* LPCWSTR, L"..." para indicar que se trata de un long char
#else
	hDLL = LoadLibrary(L"..\\GameToLoad\\Juego_d");	// typedef const wchar_t* LPCWSTR, L"..." para indicar que se trata de un long char
#endif

	if (NULL != hDLL)
	{
		lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL, "LoadGame");
		if (NULL != lpfnDllFunc1)
		{
			lpfnDllFunc1(NULL, NULL);
		}
		else throw "Function LoadGame not found in DLL";

		// Libera la memoria de la DLL cargada explicitamente
		FreeLibrary(hDLL);
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
bool Motor::loadMainMenu(std::string name,const char*get) {
	try {
		// Borra las entidades de la escena actual
		Singleton<EntidadManager>::instance()->pauseEntidades();

		// Devuelve la ruta de la escena
		std::string sceneRoute = Singleton<LoadResources>::instance()->scene(name).c_str();

		// Lee la escena cargando todas las entidades y sus componentes
		readFileMenus(sceneRoute,get);
		Singleton<OverlayManager>::instance()->setCallBackToButton("NewGamePanel", newGame);
		Singleton<OverlayManager>::instance()->setCallBackToButton("OptionsPanel",options);
		Singleton<OverlayManager>::instance()->setCallBackToButton("ExitPanel", salir);
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
	//// Entidad con un transform
	//Entidad* ent = Singleton<EntidadManager>::instance()->addEntidad();
	//map<string, string> m;
	//ent->addComponent("transform", m);

}

bool Motor::getStop()
{
	return stop;
}

void Motor::salir(Motor* m)
{
	m->stop = true;
}

void Motor::newGame(Motor* m)
{
	//OverlayManager* o = Singleton<OverlayManager>::instance();
	//delete o;
	//o = nullptr;
	Singleton<OverlayManager>::close();
	m->loadScene("TestScene.lua");
}
void Motor::options(Motor* m)
{
	//OverlayManager* o = Singleton<OverlayManager>::instance();
	//delete o;
	//o = nullptr;
	//Singleton<OverlayManager>::close();
	Singleton<OverlayManager>::instance()->clear();
	m->loadMainMenu("Options.lua","GetOptions");
}

//void Motor::deleteOverlay(Motor* m)
//{
//	delete m->_overlayManager;
//	m->_overlayManager = nullptr;
//}

void Motor::setStop(bool s)
{
	stop = s;
}

