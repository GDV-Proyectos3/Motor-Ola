#include "Motor.h"
#include "OgreManager.h"
#include "Componente.h"
#include "ComponenteFactoria.h"
#include "ComponenteRegistro.h"
#include "utils/Singleton.h"
#include "InputManager.h"

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
	if(!_ogreManager) _ogreManager = new OgreManager();	
	if (!_inputManager)_inputManager = new InputManager();
}

Motor::~Motor()
{
	// Destruye los managers
	if (_ogreManager) delete _ogreManager;
	if (_inputManager)delete _inputManager;
}

void Motor::initSystems()
{
	_ogreManager->init();
	_inputManager->init(this);
	
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

