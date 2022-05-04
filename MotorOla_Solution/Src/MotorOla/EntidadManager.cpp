#include "EntidadManager.h"
#include "Entidad.h"

std::unique_ptr<EntidadManager> Singleton<EntidadManager>::instance_ = nullptr;
void EntidadManager::update()
{
	for (int i = 0; i < entitys_.size(); i++)
	{
		if (entitys_.at(i) != nullptr) {
			(*entitys_.at(i)).update();
		}
	}
}

void EntidadManager::draw()
{
	for (auto& e : entitys_)
		e->draw();
}

void EntidadManager::refresh()
{
	entitys_.erase( std::remove_if ( entitys_.begin(), entitys_.end(), [](const uptr_ent& e) { return !e->isActive(); }), entitys_.end());
}

void EntidadManager::pauseEntidades()
{
	for (auto& e : entitys_)
		e->setActive(false);
}

void EntidadManager::reanudeEntidades()
{
	for (auto& e : entitys_)
		e->setActive(false);
}

void EntidadManager::sincronizaVectorEnt()
{
	entitys_.resize(entitys_.size());
}

Entidad* EntidadManager::getEntidadByID(int id)
{
	// Busca entre las entidades activas
	Entidad* e = nullptr;
	auto it = entitys_.begin();
	while (e == nullptr && it != entitys_.end()) {
		if ((*it)->getID() == id) {
			e = (*it).get();
		}
		++it;
	}
	return e;
}

Entidad* EntidadManager::addEntidad()
{
	Entidad* e = new Entidad();
	e->setEntityMngr(this);
	uptr_ent uPtr(e);
	entitys_.emplace_back(std::move(uPtr));
	return e;
}

Entidad* EntidadManager::addEntidad(std::string entityName, int id)
{
	Entidad* e = new Entidad(entityName, id);
	e->setEntityMngr(this);
	uptr_ent uPtr(e);
	entitys_.emplace_back(std::move(uPtr));
	return e;
}