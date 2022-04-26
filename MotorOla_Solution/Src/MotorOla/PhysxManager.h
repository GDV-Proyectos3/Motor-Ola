#pragma once

#include "utils\Singleton.h"
#include <PxPhysicsAPI.h>
#include <vector>
#include <ctype.h>
#include <cassert>
#include "Entidad.h"
///#include <callbacks.hpp>

using namespace physx;
//#include <string>

class PhysxManager : public Singleton<PhysxManager> {
	friend Singleton<PhysxManager>;
	//using uptr_collider = std::unique_ptr<Px>;
public:	
	~PhysxManager();

	void renderCallback();

	// MAIN SINGLETON
	void init();
	void update(bool interactive, double t);
	void close(bool interactive);

	// utils
	void stepPhysics(bool interactive, double t);
	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

	// TIMER
	double GetLastTime();
	double GetCounter();
	void StartCounter();

	// factory
	void createBall(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity/* = PxVec3(0)*/);
	void attachBola(Entidad* ball);

	// Getters
	PxPhysics* getPhysX() { return mPhysics; };

	// Etc
	// virtual void patata(...) {}

	// Funciones
	// void loadPatatas();

private:
	PhysxManager(/*...*/);
	PhysxManager(bool n) { _patata = n; };

	// Timer...
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
	__int64 CounterLast = 0;

	// Variables
	bool _patata;

	PxTolerancesScale scale;

	PxFoundation* mFoundation = NULL;
	PxPhysics* mPhysics = NULL;

	PxCooking* mCooking = NULL;

	PxCudaContextManager* mCuda = NULL;
	PxCudaContextManagerDesc cudaDesc;

	PxDefaultCpuDispatcher* mDispatcher = NULL;
	PxScene* mScene = NULL;

	///ContactReportCallback gContactReportCallback;

	PxPvd* mPvd = NULL;

	PxDefaultErrorCallback mErrorCallback;
	PxDefaultAllocator mAllocator;

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
inline PxPhysics* physX() {
	return PhysxManager::instance()->getPhysX();
}