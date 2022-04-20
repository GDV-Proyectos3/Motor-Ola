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

// Componentes
#include "Entidad.h"
#include "Componente.h"
#include "ComponenteFactoria.h"
#include "ComponenteRegistro.h"
#include "Transform.h"
#include "Mesh.h"
#include "Movible.h"
#include "AudioSource.h"

typedef HRESULT(CALLBACK* LPFNDLLFUNC1)(DWORD, UINT*);

Motor::Motor()
{
	// Inicia los managers
	Singleton<LoadResources>::instance();
	Singleton<OgreManager>::instance();	
	Singleton<EntidadManager>::instance();
	Singleton<FMODAudioManager>::instance();
	Singleton<OverlayManager>::instance();
}

Motor::~Motor()
{
	// Destruye los managers en orden inverso a la creaci�n
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
	
	// Se registran los componentes que conoce el motor
	registryComponents();

	// El motor intenta cargar un juego, pero si hay algun error se arranca con la funcion loadTestMotorGame
	try {
		loadDLLGame();
	}
	catch (const char* error) {
		std::cout << "Error: " << error << "\n";
		loadTestMotorGame();
	}

	// Carga una escena con Lua
	if(!loadScene(""))
		throw "Error loading a Scene\n";
}

void Motor::registryComponents()
{
	// Apuntar aqu� todos los componentes del motor (apuntar solo despues de refactorizar)
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

		// Actualizar las f�sicas de las entidades
		//_physxManager->update();

		// Actualiza los transforms de las entitys despu�s de las f�sicas
		if (Singleton<OverlayManager>::instance() != nullptr) {
			Singleton<OverlayManager>::instance()->update();
		}
		// Actualiza el resto de componentes (tambi�n los del juego)
		Singleton<EntidadManager>::instance()->update();

		// Renderiza las entidades
		Singleton<OgreManager>::instance()->update();

		// Contador de frames que los muetra cada 100 frames
#if (defined _DEBUG)
		if (++frame % 100 == 0) {
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

	hDLL = LoadLibrary(L"..\\GameToLoad\\Juego");	// typedef const wchar_t* LPCWSTR, L"..." para indicar que se trata de un long char

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

		// BORRAR ES TESTEO
		name = "run.lua";
		std::string testSceneRoute = Singleton<LoadResources>::instance()->scene(name).c_str();
		readFileTest(testSceneRoute);

		// TO DO
		// Crea la ruta de la escena (esto deber�a venir de Resources pero para ir probando)
		name = "TestScene.lua";
		std::string sceneRoute = Singleton<LoadResources>::instance()->scene(name).c_str();
		std::cout << "OgreManager is at " << Singleton<OgreManager>::instance() << "\n";
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

void Motor::loadTestMotorGame() 
{
	// Entidad con un transform
	Entidad* ent = Singleton<EntidadManager>::instance()->addEntidad();
	map<string, string> m;
	ent->addComponent("transform", m);

}

bool Motor::getStop()
{
	return stop;
}

void Motor::salir(Motor* m)
{
	m->stop = true;
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

