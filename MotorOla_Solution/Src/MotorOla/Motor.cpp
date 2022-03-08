#include "Motor.h"
#include "OgreManager.h"
#include "utils/Singleton.h"

Motor::Motor()
{
	// Inicia los managers
	if(!_ogreManager) _ogreManager = new OgreManager();	
}

Motor::~Motor()
{
	// Destruye los managers
	if (_ogreManager) delete _ogreManager;
}

void Motor::initSystems()
{

	_ogreManager->init();
	
}

void Motor::updateSystems()
{
	//Actualiza el motor. Bucle input->update/fisicas->render
	while (!stop) {
		handleEvents();
		//Input
		//Update
		//Render
		
		
	}
}
