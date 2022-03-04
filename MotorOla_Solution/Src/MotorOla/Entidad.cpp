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
		c->update();
	}

	for (auto& c : components)
	{
		c->draw();
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
