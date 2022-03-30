#pragma once

#include <SDL.h>
#include "utils\Singleton.h"
#include <iostream>

class Motor;

class InputManager : public Singleton<InputManager> {
	friend Singleton<InputManager>;
public:
	~InputManager();
	//void init(Motor* mo);
	void handleEvents();
private:
	InputManager() : m(nullptr) {};
	InputManager(Motor* mo)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		m = mo;
	}
	Motor* m;

};

// Esta macro define una forma compacta para usar el Singleton InputManager, 
// en lugar de escribir 'InputManager::instance()->method()' escribiremos 'im().method()'
//
inline InputManager& im() {
	return *InputManager::instance();
}