#include "EntidadManager.h"
#include "Entidad.h"



void EntidadManager::update()
{
	for (auto& e : entitys_)
		e->update();
}

void EntidadManager::draw()
{
	for (auto& e : entitys_)
		e->draw();
}

void EntidadManager::refresh()
{
	entitys_.erase( std::remove_if ( 
		entitys_.begin(),
		entitys_.end(), 
		[](const uptr_ent& e) { return !e->isActive();	}),
		entitys_.end()
	);

}

Entidad* EntidadManager::addEntidad()
{
	Entidad* e = new Entidad();
	e->setEntityMngr(this);
	uptr_ent uPtr(e);
	entitys_.emplace_back(std::move(uPtr));
	return e;
}
