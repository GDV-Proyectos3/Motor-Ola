#include "Motor.h"
#include "OgreManager.h"
#include "utils/Singleton.h"
#include "InputManager.h"
#include "LoadResources.h"
//#include "FMODAudioManager.h"



Motor::Motor()
{
	// Inicia los managers
	if (!_loadResources)_loadResources = new LoadResources();
	if(!_ogreManager) _ogreManager = new OgreManager();	
	if (!_inputManager)_inputManager = new InputManager();
	//if (!_audioManager)_audioManager = new FMODAudioManager();
	
	
}

Motor::~Motor()
{
	// Destruye los managers
	if (_ogreManager) delete _ogreManager;
	if (_inputManager)delete _inputManager;
	if (_loadResources)delete _loadResources;
	//if (_audioManager)delete _audioManager;
}

void Motor::initSystems()
{
	_loadResources->init();
	std::cout << _loadResources->mes("ogrehead.mesh") << std::endl;
	_ogreManager->init();
	_inputManager->init(this);
	/*_audioManager->init();
	_audioManager->loadMusic(0, _loadResources->aud("blind_shift.mp3").c_str());
	_audioManager->playMusic(0, true);*/
	
	
	
	
	
	
	
	
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

