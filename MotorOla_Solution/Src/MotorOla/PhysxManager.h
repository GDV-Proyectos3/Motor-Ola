#pragma once

#include "utils\Singleton.h"
#include "Transform.h"
#include <PxPhysicsAPI.h>
#include <vector>
#include <cassert>

using namespace physx;

class PhysxManager : public Singleton<PhysxManager> {
	friend Singleton<PhysxManager>;
	friend std::ostream& operator<<(std::ostream& os, const PxVec3& v);
public:	
	~PhysxManager();

	// main resources
	void runPhysX();

	// transform
	PxTransform globalToPhysxTR(Transform& tr);
	Transform physxToGlobalTR(const PxRigidActor& body);
	
	// elimina memoria
	void releaseScene();
	void releaseBody(PxActor& body);

	// MAIN SINGLETON
	void init();
	void update(bool interactive, double t);
	void close(bool interactive);

	// utils
	void stepPhysics(bool interactive, double t);
	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

	// TIMER
	void StartCounter();
	double GetCounter();
	double GetLastTime();
	void debugTime();

	// FACTORY
	PxRigidDynamic* createDynamic(const PxTransform& transform, const PxGeometry& geometry, PxMaterial& material, const PxVec3& velocity = PxVec3(0));
	PxRigidDynamic* createDynamic(const PxTransform& transform, PxShape* shape, const PxVec3& velocity = PxVec3(0));
	
	PxRigidStatic* createStaticRigid(const PxTransform& transform, const PxGeometry& geom, PxMaterial& material);
	PxRigidStatic* createStaticRigid(const PxTransform& transform, PxShape* shape);

	PxShape* createShape(const PxGeometry& geom, PxMaterial& material, bool isExclusive = false);
	PxShape* createTriggerShape(const PxGeometry& geom, PxMaterial& material, bool isExclusive = false);

	// factory prefabs
	PxRigidStatic* createTriggerStaticBox(const PxVec3 halfExtent = PxVec3(10.0f, 1.0f, 10.0f), const PxTransform& transform = PxTransform(0.0f, 10.0f, 0.0f));
	PxRigidDynamic* createBall();
	void createStackBoxes(const PxTransform& t, PxU32 size, PxReal halfExtent);
	void tiledStacks(PxReal num = 5, PxReal sideLength = 1.0f);

	// Getters
	PxPhysics* getPhysX() { return mPhysics; };
	PxRigidDynamic* getBall() { return testBALL; };

private:
	PhysxManager(/*...*/);
	PhysxManager(bool n) { _patata = n; };

	PxRigidDynamic* testBALL = nullptr;

	// ON/OFF physics
	bool	mPause = false;
	bool	mOneFrame = false;

	// Timer...
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
	__int64 CounterLast = 0;
	__int64 GlobalTimer = 0;

	// Variables editables
	bool _patata;

	PxTolerancesScale scale;
	PxCudaContextManagerDesc cudaDesc;

	PxReal stackZ = 10.0f;

	// Inevitables para que funcione
	PxDefaultAllocator		mAllocator;
	PxDefaultErrorCallback	mErrorCallback;

	PxFoundation* mFoundation = NULL;
	PxPhysics* mPhysics = NULL;

	PxDefaultCpuDispatcher* mDispatcher = NULL;
	PxScene* mScene = NULL;

	PxMaterial* mMaterial = NULL;	// default material

	PxPvd* mPvd = NULL;
	PxCooking* mCooking = NULL;

	PxCudaContextManager* mCuda = NULL;
};

// Esta macro define una forma compacta para usar el Singleton PhysxManager, 
// en lugar de escribir 'PhysxManager::instance()->method()' escribiremos 'im().method()'
inline PhysxManager& pm() {
	return *PhysxManager::instance();
}

// Forma breve de acceder al creador y padre todopoderoso de las fisicas
inline PxPhysics* physX() {
	return PhysxManager::instance()->getPhysX();
}
