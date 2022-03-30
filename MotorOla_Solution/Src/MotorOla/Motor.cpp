#include "Motor.h"
#include "OgreManager.h"
#include "Componente.h"
#include "ComponenteFactoria.h"
#include "ComponenteRegistro.h"
//#include "utils/Singleton.h"
#include "PhysxManager.h"
#include "InputManager.h"
#include "Entidad.h"
#include <Windows.h>
#include "AudioSource.h"


typedef HRESULT(CALLBACK* LPFNDLLFUNC1)(DWORD, UINT*);

#include "LoadResources.h"
#include "FMODAudioManager.h"


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
	if (!_loadResources)_loadResources = new LoadResources();
	if(!_ogreManager) _ogreManager = new OgreManager();	
	//if (!_inputManager)_inputManager = new InputManager();
	_inputManager->init(this);

	if (!_audioManager)_audioManager = new FMODAudioManager();

	// Pablo Cubells dice: "Al ser un Singleton no hace se crea así la instancia,
	// como una clase cualquiera, el new se hace dentro de la clase Singleton en init().
	// Esto es porque es un unique_pointer."
	// if (!_physxManager)_physxManager = new PhysxManager();
	_physxManager->init();
	
	

}

Motor::~Motor()
{
	// Destruye los managers
	if (_ogreManager) delete _ogreManager;
	//if (_inputManager)delete _inputManager;
	if (_loadResources)delete _loadResources;
	if (_audioManager)delete _audioManager;

	// Pablo Cubells dice: "dejo comentado este delete porque no creo que se haga así,
	// lo que creo es que no hay que hacer nada y se elimina solo al cerrar la aplicación, 
	// pero por si acaso, hasta que esté claro, dejo esto."
	// if (_physxManager)delete _physxManager; 
}

void Motor::initSystems()
{

	_loadResources->init();
	//std::cout << _loadResources->mes("ogrehead.mesh") << std::endl;
	_ogreManager->init();
	//_inputManager->init(this);
	
	// Pablo Cubells dice: "esto es en el caso que mantegamos esta función,
	// podrían pasarse las inicializaciones al constructor."
	// _physxManager->init();

	_audioManager->init();
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
		im().handleEvents();
		pm().update();
		_ogreManager->update();
		
	
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

