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
	// Inicia los managers
	if (!_loadResources)_loadResources = new LoadResources();
	if(!_ogreManager) _ogreManager = new OgreManager();	
	//if (!_inputManager)_inputManager = new InputManager();
	if (!_entidadManager) _entidadManager = new EntidadManager();
	if (!_audioManager)_audioManager = new FMODAudioManager();
	if (!_overlayManager)_overlayManager = new OverlayManager();
}

Motor::~Motor()
{
	// Destruye los managers
	
	//if (_inputManager) delete _inputManager;
	if (_loadResources) delete _loadResources;
	if (_audioManager) delete _audioManager;
	if (_entidadManager) delete _entidadManager;
	if (_ogreManager) delete _ogreManager;
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
	
	// Registrando Componentes
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
	loadScene("");
}

void Motor::registryComponents()
{
	ComponenteRegistro::ComponenteRegistro<Transform>("transform");
	//ComponenteRegistro::ComponenteRegistro<Mesh>("mesh");
	//ComponenteRegistro::ComponenteRegistro<Movible>("movible");
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

		// Actualiza los transforms de las entitys de Ogre
		//for (auto e = _entidadManager->entitys_.begin(); e  != _entidadManager->entitys_.end(); e++)
		//{
		//	
		//}

		//Ogre::Vector3 v;
		////v.x = ent3->getComponent<Transform>()->getPos().getX();
		////v.y = ent3->getComponent<Transform>()->getPos().getY();
		////v.z;		
		//v.x = ent2->getParentNode()->getPosition().x + 0.1;
		//v.y = ent2->getParentNode()->getPosition().y;
		//v.z = ent2->getParentNode()->getPosition().z;

		//ent2->getParentNode()->setPosition(v);

		// Renderiza las entidades
		_ogreManager->update();


		//std::cout << "Vuelta " << ++frame << "\n";
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
		name = "TestScene.lua";
		// Borra las entidades de la escena actual
		Singleton<EntidadManager>::instance()->pauseEntidades();

		// TO DO
		// Crea la ruta de la escena (esto debería venir de Resources pero para ir probando)
		std::string sceneRoute = _loadResources->scene(name).c_str();

		//readFile(sceneRoute);
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

