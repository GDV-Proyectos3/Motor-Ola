#pragma once
#include "ECS.h"
#include <map>
#include <string>

class Entidad;

class Componente
{
	friend Entidad;

public:
	virtual ~Componente() {};
	virtual bool init(const std::map<std::string, std::string>& mapa) = 0;

	virtual void start() {};
	virtual void update() {};

	

	//Para triggers
	virtual void onTriggerStart(Entidad* other) {};
	virtual void onTriggerStay(Entidad* other) {};
	virtual void onTriggerEnd(Entidad* other) {};

	//Para las Colisiones
	virtual void onCollisionStart(Entidad* other) {};
	virtual void onCollisionStay(Entidad* other) {};
	virtual void onCollisionEnd(Entidad* other) {};

	virtual void draw() {};

	inline bool setActive(bool state) { _active = state; }
	inline bool setInitialized(bool state) { _inicializado = state; }
	
	inline void setEntidad(Entidad* entity)	{ _entity = entity;	}
	inline Entidad* getEntidad() const { return _entity; }

protected:
	bool _active = true;
	bool _inicializado= false;
	Entidad* _entity = nullptr;
};


