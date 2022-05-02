#include "Entidad.h"
#include "Transform.h"
#include "LoadResources.h"
#include "EntidadManager.h"
#include "PhysxManager.h"
#include "LuaReader.h"
#include "RigidBody.h"

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

void Entidad::OnCollisionEnter(Entidad* other)
{
	std::cout << "OnCollisionEnter\n";
	for (auto& c : components)
	{
		c->onCollisionStart(other);
	}
}

void Entidad::OnTriggerEnter(Entidad* other)
{
	std::cout << "OnTriggerEnter\n";
	for (auto& c : components)
	{
		c->onTriggerStart(other);
	}
}

bool Entidad::init()
{
	_numTriesToLoad = components.size() * 100000;
	int i = 0;
	j = 0;
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
	if (ent->getComponent<RigidBody>() != nullptr) {
		pm().setGlobalToPhysxTR(*ent, *ent->getComponent<RigidBody>()->getBody());
	}

	return ent;
}
