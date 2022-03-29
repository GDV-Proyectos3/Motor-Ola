#include "InputManager.h"
#include "Motor.h"
#include "Entidad.h"
#include "Transform.h"


InputManager::InputManager() {

}

InputManager::~InputManager()
{

}

void InputManager::init(Motor* mo)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	m = mo;
}

void InputManager::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)&& !m->getStop()) {
		if (event.type == SDL_QUIT) {
			//m->setStop (true);
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				m->setStop(true);
			}
			else if (event.key.keysym.sym == SDLK_0) {
				std::cout << "Input" << std::endl;
			}

			// Para pruebas
			if (event.key.keysym.sym == SDLK_w) {
				std::cout << "W\n";
				//std::cout << pala1->getComponent<Transform>()->getPos() << "\n";
				pala1->getComponent<Transform>()->setPosY(pala1->getComponent<Transform>()->getPos().getY() + 5.0f);
				std::cout << pala1->getComponent<Transform>()->getPos() << "\n";
			}
			else if (event.key.keysym.sym == SDLK_s) {
				std::cout << "S\n";
				pala1->getComponent<Transform>()->setPosY(pala1->getComponent<Transform>()->getPos().getY() - 5.0f);
			}
		}
	}
}

void InputManager::setPala2(Entidad* e)
{
	pala2 = e;
}

void InputManager::setPala1(Entidad* e) {
	pala1 = e;
}
