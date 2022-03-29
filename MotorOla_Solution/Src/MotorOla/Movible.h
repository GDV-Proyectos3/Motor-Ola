#pragma once
#include "Componente.h"
#include "./utils/Vector2D.h"
#include "utils/Vector2D.h"
#include "Transform.h"
#include "Entidad.h"

class Movible : public Componente
{
public:
	Movible() {};
	virtual ~Movible() {};

	bool  getActive() {};
	void  setActive(bool a) {};

	void init() {};
	void update() {
		Entidad* e = getEntidad();
		Transform* t = e->getComponent<Transform>();
		t->translate(0.2, 0.0, 0.0);
	};
	//void draw();

private:
	bool active = true;
};

