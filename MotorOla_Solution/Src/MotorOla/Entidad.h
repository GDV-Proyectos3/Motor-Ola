#pragma once

#include <vector>
#include "Componente.h"

class EntidadManager;
class Entidad
{
	using uptr_cmp = std::unique_ptr<Componente>;
public:
	Entidad();
	void update();
	void draw() {};
	bool isActive() const;
	void destroy();

	virtual ~Entidad() {};

	inline EntidadManager* getEntityMngr() const {
		return entManager_;
	}

	inline void setEntityMngr(EntidadManager* mngr) {
		entManager_ = mngr;
	}

	template<typename  T>
	inline bool hasComponent() const {
		return componentBitset[getComponentTypeID<T>];
	}

	template<typename T>
	inline T* getComponent() const {
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return static_cast<T*>(ptr);
	}


	template<typename T, typename ... Targs>
	inline T* addComponent(Targs&&...args) {

		T* c(new T(std::forward<Targs>(args)...));
		uptr_cmp uPtr(c);
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()]=c;
		componentBitset[getComponentTypeID<T>()] = true;


		c->setEntity(this);
		c->init();
		return c;
	}

private:
	EntidadManager* entManager_;
	std::vector<uptr_cmp> components;
	bool active = true;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;
};

