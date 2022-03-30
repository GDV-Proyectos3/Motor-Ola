#pragma once

#include "utils\Singleton.h"

//#include <string>

#include <PxPhysicsAPI.h>

namespace physx {
	// ...
}

class PhysxManager : public Singleton<PhysxManager> {
	friend Singleton<PhysxManager>;
public:	
	~PhysxManager();

	// Getters
	// ...

	void update();
	void close();

	// Etc
	// virtual void patata(...) {}

	// Funciones
	// void loadPatatas();

private:
	PhysxManager(/*...*/) : _patata(false) {};
	PhysxManager(bool n) { _patata = n; };
	// Variables
	bool _patata;
};

// Esta macro define una forma compacta para usar el Singleton PhysxManager, 
// en lugar de escribir 'PhysxManager::instance()->method()' escribiremos 'im().method()'
//
inline PhysxManager& pm() {
	return *PhysxManager::instance();
}
