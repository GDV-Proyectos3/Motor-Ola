#pragma once

#include "utils\Singleton.h"
#include <PxPhysicsAPI.h>
#include "PhysxErrorCallback.h"

using namespace physx;
//#include <string>

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
	PhysxManager(/*...*/);
	PhysxManager(bool n) { _patata = n; };
	// Variables
	bool _patata;

	PxFoundation* gFoundation = NULL;
	PxDefaultAllocator		gAllocator;

	// Pablo Cubells dice: "IDK why pero esta mierda así funciona bien."
	// PxDefaultErrorCallback	gErrorCallback;
	PhysxErrorCallback	gErrorCallback;
};

// Esta macro define una forma compacta para usar el Singleton PhysxManager, 
// en lugar de escribir 'PhysxManager::instance()->method()' escribiremos 'im().method()'
//
inline PhysxManager& pm() {
	return *PhysxManager::instance();
}
