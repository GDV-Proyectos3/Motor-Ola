#include "InputManager.h"
#include "Motor.h"
//InputManager::InputManager(Motor* mo) 
//{
//	SDL_Init(SDL_INIT_EVERYTHING);
//	m = mo;
//}
InputManager::~InputManager()
{
	m = nullptr;
}

//void InputManager::init(Motor* mo)
//{
//	SDL_Init(SDL_INIT_EVERYTHING);
//	m = mo;
//}

void InputManager::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)&& !m->getStop()) {

		if (event.type == SDL_QUIT) {

			m->setStop (true);
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				m->setStop(true);
			}
			else if (event.key.keysym.sym == SDLK_0) {
				std::cout << "Input" << std::endl;
			}
		}


	}
}
