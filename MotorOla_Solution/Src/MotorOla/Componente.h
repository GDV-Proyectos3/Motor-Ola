#pragma once

#include "ECS.h"

class Entidad;

class Componente
{
public:
	virtual ~Componente() {}
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};

	inline void setEntidad(Entidad* entity)	{
		entity_ = entity;
	}
protected:
	Entidad* entity_ = nullptr;

};


