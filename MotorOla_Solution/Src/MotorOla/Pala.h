#pragma once
#include "Componente.h"
#include "./utils/Vector2D.h"
#include "utils/Vector2D.h"
#include "InputManager.h"
#include "Transform.h"
#include "Entidad.h"

class Pala : public Componente
{
public:
	Pala() {};
	virtual ~Pala() {};

	bool  getActive() {};
	void  setActive(bool a) {};

	void init() { /*InputManager::instance()->setPala1(entity_);*/ };
	void update() {
		

		//Entidad* e = getEntidad();
		//Transform* t = e->getComponent<Transform>();
		//t->translate(0.2, 0.0);
	};
	//void draw();

private:
	bool active = true;
};



