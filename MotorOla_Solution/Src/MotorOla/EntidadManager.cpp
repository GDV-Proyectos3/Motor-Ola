#include "EntidadManager.h"
#include "Entidad.h"

std::unique_ptr<EntidadManager> Singleton<EntidadManager>::instance_ = nullptr;
void EntidadManager::update()
{
	for (int i = 0; i < _entitys.size(); i++)
	{
		if (_entitys.at(i) != nullptr) {
			(*_entitys.at(i)).update();
		}
	}
}

void EntidadManager::draw()
{
	for (auto& e : _entitys)
		e->draw();
}

void EntidadManager::refresh()
{
	_entitys.erase( std::remove_if ( _entitys.begin(), _entitys.end(), [](const uptr_ent& e) { return !e->isActive(); }), _entitys.end());
}

void EntidadManager::pauseEntidades()
{
	for (auto& e : _entitys)
		e->setActive(false);
}

void EntidadManager::reanudeEntidades()
{
	for (auto& e : _entitys)
		e->setActive(false);
}

void EntidadManager::sincronizaVectorEnt()
{
	_entitys.resize(_entitys.size());
}

Entidad* EntidadManager::getEntidadByID(int id)
{
	// Busca entre las entidades activas
	Entidad* e = nullptr;
	auto it = _entitys.begin();
	while (e == nullptr && it != _entitys.end()) {
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
	_entitys.emplace_back(std::move(uPtr));
	return e;
}

Entidad* EntidadManager::addEntidad(std::string entityName, int id)
{
	Entidad* e = new Entidad(entityName, id);
	e->setEntityMngr(this);
	uptr_ent uPtr(e);
	_entitys.emplace_back(std::move(uPtr));
	return e;
}