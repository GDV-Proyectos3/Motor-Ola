#include "Motor.h"
#include "OgreManager.h"
#include "utils/Singleton.h"
#include "InputManager.h"
#include "LoadResources.h"



Motor::Motor()
{
	// Inicia los managers
	if (!_loadResources)_loadResources = new LoadResources();
	if(!_ogreManager) _ogreManager = new OgreManager();	
	if (!_inputManager)_inputManager = new InputManager();
	
	
}

Motor::~Motor()
{
	// Destruye los managers
	if (_ogreManager) delete _ogreManager;
	if (_inputManager)delete _inputManager;
	if (_loadResources)delete _loadResources;
}

void Motor::initSystems()
{
	_loadResources->init();
	std::cout << _loadResources->mes("ogrehead.mesh") << std::endl;
	_ogreManager->init();
	_inputManager->init(this);
	
	
	
	
	
	
	
	
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
bool Motor::getStop()
{
	return stop;
}
void Motor::setStop(bool s)
{
	stop = s;
}

