#pragma once

#include "ECS.h"

class Entidad;

class Componente
{
	friend Entidad;

public:
	virtual void init() {};

	virtual void update() {};

	virtual void onTriggerStart(Entidad* other) {};
	virtual void onTriggerStay(Entidad* other) {};
	virtual void onTriggerEnd(Entidad* other) {};

	virtual void onCollisionStart(Entidad* other) {};
	virtual void onCollisionStay(Entidad* other) {};
	virtual void onCollisionEnd(Entidad* other) {};

	virtual void draw() {};

	virtual ~Componente() {}

	inline void setEntidad(Entidad* entity)	{ entity_ = entity;	}

	inline bool setActive(bool state) { active_ = state; }

protected:
	bool active_ = true;
	bool inicializado_ = true;
	Entidad* entity_ = nullptr;

};


