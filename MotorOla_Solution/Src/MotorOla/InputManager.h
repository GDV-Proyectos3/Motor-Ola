#pragma once

#include <SDL.h>
#include "utils\Singleton.h"
#include <iostream>

class Motor;
class Entidad;

class InputManager : public Singleton<InputManager> {
	friend Singleton<InputManager>;
public:
	InputManager();
	~InputManager();
	void init(Motor* mo);
	void handleEvents();
	void setPala1(Entidad* e);
	void setPala2(Entidad* e);
private:
	Motor* m;


	// Pruebas
	Entidad* pala1 = nullptr;
	Entidad* pala2 = nullptr;
};