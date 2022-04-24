#pragma once

#include "utils\Singleton.h"
#include <PxPhysicsAPI.h>
#include <vector>
#include "Entidad.h"
///#include <callbacks.hpp>

using namespace physx;
//#include <string>

class PhysxManager : public Singleton<PhysxManager> {
	friend Singleton<PhysxManager>;
	//using uptr_collider = std::unique_ptr<Px>;
public:	
	~PhysxManager();

	// Getters
	// ...
	void init() {};
	void update();
	void close();

	void createBall(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity/* = PxVec3(0)*/);
	void attachBola(Entidad* ball);

	// Getters
	PxPhysics* getPhysX() { return gPhysics; };

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

	PxPhysics* gPhysics = NULL;

	PxDefaultCpuDispatcher* gDispatcher = NULL;

	PxScene* gScene = NULL;

	///ContactReportCallback gContactReportCallback;

	PxPvd* gPvd = NULL;

	PxDefaultAllocator gAllocator;

	PxDefaultErrorCallback gErrorCallback;

	//std::vector<uptr_collider> colliders_;


	Entidad* bola = nullptr;
};

// Esta macro define una forma compacta para usar el Singleton PhysxManager, 
// en lugar de escribir 'PhysxManager::instance()->method()' escribiremos 'im().method()'
//
inline PhysxManager& pm() {
	return *PhysxManager::instance();
}

// Forma breve de acceder al creador y padre todopoderoso de las fisicas
inline PxPhysics* physx() {
	return PhysxManager::instance()->getPhysX();
}
