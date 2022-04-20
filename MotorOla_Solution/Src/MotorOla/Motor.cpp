#include "Motor.h"
#include "OgreManager.h"
#include "Componente.h"
#include "ComponenteFactoria.h"
#include "ComponenteRegistro.h"
#include "utils/Singleton.h"
#include "InputManager.h"
#include "Mesh.h"
#include "Transform.h"
#include "Entidad.h"
#include "EntidadManager.h"
#include "Movible.h"
#include <Windows.h>
#include "AudioSource.h"
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreVector.h>
#include "LoadResources.h"
#include "FMODAudioManager.h"
#include <Ogre.h>

#include "LuaReader.h"

#include "PingPongCtrl.h"
#include "OverlayManager.h"

typedef HRESULT(CALLBACK* LPFNDLLFUNC1)(DWORD, UINT*);

Motor::Motor()
{
	// Creo que esto mal
	//if(!_ogreManager) _ogreManager = new OgreManager();	

	// Inicia los managers
	if (!_loadResources)_loadResources = Singleton<LoadResources>::instance();
	std::cout << "-----Load at " << _loadResources << "\n";
	if(!_ogreManager) _ogreManager = Singleton<OgreManager>::instance();	
	std::cout << "-----Ogre at " << _ogreManager << "\n";
	if (!_entidadManager) _entidadManager = Singleton<EntidadManager>::instance();
	std::cout << "-----EntidadManager at " << _entidadManager << "\n";
	if (!_audioManager)_audioManager = Singleton<FMODAudioManager>::instance();
	if (!_overlayManager)_overlayManager = Singleton<OverlayManager>::instance();
	//if (!_inputManager)_inputManager = new InputManager();
}

Motor::~Motor()
{
	// Destruye los managers
	if (_loadResources) delete _loadResources;
	if (_audioManager) delete _audioManager;
	if (_entidadManager) delete _entidadManager;
	if (_ogreManager) delete _ogreManager;
	//if (_inputManager) delete _inputManager;
	//if (_overlayManager) delete _overlayManager;
	//SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Motor::initSystems()
{
	_loadResources->init();
	_ogreManager->init();
	//_inputManager->init(this);
//	_inputManager->init();
	_audioManager->init();
    _overlayManager->init(_ogreManager,this);
	
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
	// Apuntar aquí todos los componentes del motor
	try {
		ComponenteRegistro::ComponenteRegistro<Transform>("transform");
		ComponenteRegistro::ComponenteRegistro<Mesh>("mesh");
		//ComponenteRegistro::ComponenteRegistro<Movible>("movible");
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
		//_inputManager->handleEvents();
		ih().clearState();
		while (SDL_PollEvent(&event))
			ih().update(event);

		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
			stop = true;
			continue;
		}

		// Actualizar las físicas de las entidades
		//_physxManager->update();

		// Actualiza los transforms de las entitys después de las físicas
		if (_overlayManager != nullptr) {
			_overlayManager->update();
		}
		// Actualiza el resto de componentes (también los del juego)
		_entidadManager->update();

		// Renderiza las entidades
		_ogreManager->update();


		// Contador de frames que los muetra cada 100 frames
		if (++frame % 100 == 0) {
			std::cout << "FrameCount: " << frame << "\n";
		}
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
		std::string testSceneRoute = _loadResources->scene(name).c_str();
		readFileTest(testSceneRoute);

		// TO DO
		// Crea la ruta de la escena (esto debería venir de Resources pero para ir probando)
		name = "TestScene.lua";
		std::string sceneRoute = _loadResources->scene(name).c_str();
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
	Entidad* ent = _entidadManager->addEntidad();
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
void Motor::deleteOverlay(Motor* m)
{
	delete m->_overlayManager;
	m->_overlayManager = nullptr;
}

void Motor::setStop(bool s)
{
	stop = s;
}

