#pragma once

#include <SDL.h>
#include "utils\Singleton.h"
#include <iostream>

class Motor;

class InputManager : public Singleton<InputManager> {
	friend Singleton<InputManager>;
public:
	InputManager();
	~InputManager();
	void init(Motor* mo);
	void handleEvents();
private:
	Motor* m;

};