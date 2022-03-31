#include "Entidad.h"

Entidad::Entidad():
	entManager_(nullptr),
	active(true)
{
}

void Entidad::update()
{
	for (auto& c : components)
	{
		// de aqui hay que quitar el update de physx

		c->update();
	}
}

bool Entidad::isActive() const
{
	return active;
}

void Entidad::destroy()
{
	active = false;
}
