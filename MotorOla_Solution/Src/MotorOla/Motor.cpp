#include "Motor.h"
#include "OgreManager.h"
#include "Componente.h"
#include "InputManager.h"
#include "LoadResources.h"
#include "FMODAudioManager.h"
#include "PhysxManager.h"
#include "utils/Singleton.h"
#include "ComponenteFactoria.h"
#include "ComponenteRegistro.h"
#include "Entidad.h"
#include "AudioSource.h"
#include <Windows.h>

typedef HRESULT(CALLBACK* LPFNDLLFUNC1)(DWORD, UINT*);

class Transform : public Componente {
public:
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
	virtual ~Transform() {}
};
class Rigidbody : public Componente {
public:
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
	virtual ~Rigidbody() {}
};

Motor::Motor()
{
	// Inicia los managers
	if (!_loadResources)
		_loadResources = new LoadResources();
	if(!_ogreManager)
		_ogreManager = new OgreManager();	
	if (!_inputManager)
		_inputManager = new InputManager();
	if (!_audioManager)
		_audioManager = new FMODAudioManager();
	if (!_physxManager)
		_physxManager = new PhysxManager();
}

Motor::~Motor()
{
	// Destruye los managers
	if (_ogreManager) {
		delete _ogreManager;
		_ogreManager = nullptr;
	}
	if (_inputManager) {
		delete _inputManager;
		_inputManager = nullptr;
	}
	if (_loadResources) {
		delete _loadResources;
		_loadResources = nullptr;
	}
	if (_audioManager) {
		delete _audioManager;
		_audioManager = nullptr;
	}
	if (_physxManager) {
		delete _physxManager;
		_physxManager = nullptr;
	}
}

void Motor::initSystems()
{
	_loadResources->init();
	//std::cout << _loadResources->mes("ogrehead.mesh") << std::endl;
	_ogreManager->init();
	_inputManager->init(this);
	_audioManager->init();
	_physxManager->init();
	Entidad* ent = new Entidad();
	ent->addComponent<AudioSource>(channel,_audioManager,_loadResources->aud("blind_shift.mp3").c_str());
	if (ent->hasComponent<AudioSource>()) {
		ent->getComponent<AudioSource>()->play();
	}
	/*_audioManager->init();
	_audioManager->loadMusic(0, _loadResources->aud("blind_shift.mp3").c_str());
	_audioManager->playMusic(0, true);*/
	

	// El motor intenta cargar un juego, pero si hay algun error se arranca con la funcion loadTestMotorGame
	try {
		loadDLLGame();
	}
	catch (const char* error) {
		std::cout << "Error: " << error << "\n";
		loadTestMotorGame();
	}

	// Registrando Componentes
	ComponenteRegistro::ComponenteRegistro<Transform>("transform");
	ComponenteRegistro::ComponenteRegistro<Rigidbody>("rigidbody");
}

void Motor::updateSystems()
{
	//Actualiza el motor. Bucle input->update/fisicas->render
	while (!stop) {
		_inputManager->handleEvents();
		_ogreManager->update();
		_physxManager->update();
		
		//Input
		//Update
		//Render
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

void Motor::loadTestMotorGame() {

}


bool Motor::getStop()
{
	return stop;
}

void Motor::setStop(bool s)
{
	stop = s;
}

