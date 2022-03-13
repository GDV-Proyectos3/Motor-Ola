#include "Motor.h"
#include "OgreManager.h"
#include "utils/Singleton.h"
#include "InputManager.h"





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

