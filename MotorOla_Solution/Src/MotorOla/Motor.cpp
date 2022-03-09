#include "Motor.h"
#include "OgreManager.h"
#include "utils/Singleton.h"





Motor::Motor()
{
	// Inicia los managers
	if(!_ogreManager) _ogreManager = new OgreManager();	
	SDL_Init(SDL_INIT_EVERYTHING);
	
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
		_ogreManager->update();
		
		//Input
		//Update
		//Render
		
		
	}

}
void Motor::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event) && !stop) {

		if (event.type == SDL_QUIT||event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE) {

			stop = true;
		}
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_0) {
			std::cout << "Input" << std::endl;
		}
		
	}
}
