#pragma once
#include "ECS.h"
#include <map>
#include <string>

class Entidad;

class Componente
{
	friend Entidad;

public:
	virtual bool init(const std::map<std::string, std::string>& mapa) = 0;

	virtual void start() {};
	virtual void update() {};

	virtual ~Componente() {};

	virtual void onTriggerStart(Entidad* other) {};
	virtual void onTriggerStay(Entidad* other) {};
	virtual void onTriggerEnd(Entidad* other) {};

	virtual void onCollisionStart(Entidad* other) {};
	virtual void onCollisionStay(Entidad* other) {};
	virtual void onCollisionEnd(Entidad* other) {};

	virtual void draw() {};

	inline bool setActive(bool state) { active_ = state; }
	inline bool setInitialized(bool state) { inicializado_ = state; }
	
	inline void setEntidad(Entidad* entity)	{ entity_ = entity;	}
	inline Entidad* getEntidad() const { return entity_; }

protected:
	bool active_ = true;
	bool inicializado_ = false;
	Entidad* entity_ = nullptr;
};


