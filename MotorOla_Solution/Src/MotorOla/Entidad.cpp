#include "Entidad.h"
#include "Transform.h"
#include "LoadResources.h"
#include "EntidadManager.h"
#include "LuaReader.h"

Entidad::Entidad():
	entManager_(nullptr),
	active(true)
{
}

Entidad::Entidad(std::string entityName, int id) :
	entManager_(nullptr), active(true), _name(entityName), _id(id)
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

bool Entidad::init()
{
	_numTriesToLoad = components.size() * 100000;
	int i = 0, j = 0;
	int numComponents = components.size();
	int initedComps = 0;
	while (initedComps != numComponents && j < _numTriesToLoad) {
		if (!compinits[i] && components[i]->init(compMaps[i])) {
			++initedComps;
			compinits[i] = true;
		}
		if (_needsOtherEntity) {
			return false;
		}
		++i;
		j++;
		i %= numComponents;
	}
	if (j >= _numTriesToLoad) {

		throw std::exception("Error al iniciar los componentes en Entity \n ");
	}
	Transform* t;
	i = 0;
	while (i < components.size() && !dynamic_cast<Transform*> (components[i].get())) {
		i++;
	}
	if (i != 0 && i < components.size()) {
		components[0].swap(components[i]);
	}
	return true;
}

Entidad* Entidad::instantiate(std::string name, Vectola3D position, Quaterniola rotation)
{
	std::string path = Singleton<LoadResources>::instance()->prefab(name);
	//Entidad* ent = Singleton<EntidadManager>::instance()->addEntidad();
	Entidad* ent = readPrefab(path);
	ent->getComponent<Transform>()->setPosition(position);
	ent->getComponent<Transform>()->setRotation(rotation);
	return ent;
}
