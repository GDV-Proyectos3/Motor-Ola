#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include "utils\Singleton.h"
#include <PxPhysicsAPI.h>
#include <vector>
#include <cassert>

#include "Entidad.h"
#include "ContactReportCallback.h"
class Transform;


using namespace physx;

MOTOR_API inline std::ostream& operator<<(std::ostream& os, const physx::PxVec3& v) {
	os << "(" << v.x << "," << v.y << "," << v.z << ")";
	return os;
}

MOTOR_API inline std::ostream& operator<<(std::ostream& os, const physx::PxQuat& q) {
	os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
	return os;
}

MOTOR_API class PhysxManager : public Singleton<PhysxManager> {
	friend Singleton<PhysxManager>;
public:	
	~PhysxManager();

	// main resources
	MOTOR_API void runPhysX();
	MOTOR_API void togglePause() { mPause != mPause; };

	// transform
	MOTOR_API PxTransform globalToPhysxTR(Transform& tr);
	MOTOR_API Transform physxToGlobalTR(const PxRigidActor& body);
	
	// elimina memoria
	MOTOR_API void releaseScene();
	MOTOR_API void releaseBody(PxActor& body);

	// MAIN SINGLETON
	MOTOR_API void init();
	MOTOR_API void update(bool interactive, double t);
	MOTOR_API void close(bool interactive);

	// utils
	MOTOR_API void stepPhysics(bool interactive, double t);
	MOTOR_API void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);
	MOTOR_API void onTrigger(physx::PxActor* actor1, physx::PxActor* actor2);

	// TIMER
	MOTOR_API void StartCounter();
	MOTOR_API double GetCounter();
	MOTOR_API double GetLastTime();

	// testing debug
	MOTOR_API void debugTime();
	MOTOR_API void debugBall();
	MOTOR_API void debugBuddy(Entidad* e);
	MOTOR_API void debugBody(PxRigidDynamic* rd);
	MOTOR_API void debugAllBodies();

	// FACTORY
	MOTOR_API PxRigidDynamic* createDynamic(const PxTransform& transform, const PxVec3& velocity = PxVec3(PxZero));
	MOTOR_API PxRigidDynamic* createDynamic(const PxTransform& transform, const PxGeometry& geometry, PxMaterial& material, const PxVec3& velocity = PxVec3(PxZero));
	MOTOR_API PxRigidDynamic* createDynamic(const PxTransform& transform, PxShape* shape, const PxVec3& velocity = PxVec3(PxZero));
	
	MOTOR_API PxRigidStatic* createStaticRigid(const PxTransform& transform);
	MOTOR_API PxRigidStatic* createStaticRigid(const PxTransform& transform, const PxGeometry& geom, PxMaterial& material);
	MOTOR_API PxRigidStatic* createStaticRigid(const PxTransform& transform, PxShape* shape);

	MOTOR_API PxShape* createShape(const PxGeometry& geom, PxMaterial& material, bool isExclusive = false);
	MOTOR_API PxShape* createTriggerShape(const PxGeometry& geom, PxMaterial& material, bool isExclusive = false);

	// factory prefabs
	MOTOR_API PxRigidStatic* createTriggerStaticBox(const PxVec3 halfExtent = PxVec3(10.0f, 1.0f, 10.0f), const PxTransform& transform = PxTransform(0.0f, 10.0f, 0.0f));
	MOTOR_API PxRigidDynamic* createBall();
	MOTOR_API void createStackBoxes(const PxTransform& t, PxU32 size, PxReal halfExtent);
	MOTOR_API void tiledStacks(PxReal num = 5, PxReal sideLength = 1.0f);

	// Getters
	MOTOR_API int getID(int k) { return _ids[k]; };
	MOTOR_API std::vector<int>* getIDs() { return &_ids; };
	MOTOR_API PxPhysics* getPhysX() { return mPhysics; };
	MOTOR_API PxRigidDynamic* getBall() { return testBALL; };
	MOTOR_API PxScene* getScene() { return mScene; };
	MOTOR_API PxMaterial* getMaterial() { return mMaterial; };
	MOTOR_API Entidad* findEntityByPxActor(PxActor* actor);

	// Setters
	MOTOR_API void addEntityID(int id) { _ids.push_back(id); };
	MOTOR_API void addEntityToEraseID(int id) { ids_erase.push_back(id); };
	MOTOR_API void setGlobalToPhysxTR(Entidad& e, PxRigidActor& body);
	MOTOR_API void setPhysxToGlobalTR(Entidad& e, PxRigidActor& body);

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
	ContactReportCallback	mContactReportCallback;

	PxFoundation* mFoundation = NULL;
	PxPhysics* mPhysics = NULL;

	PxDefaultCpuDispatcher* mDispatcher = NULL;
	PxScene* mScene = NULL;

	PxMaterial* mMaterial = NULL;	// default material

	PxPvd* mPvd = NULL;
	PxCooking* mCooking = NULL;

	PxCudaContextManager* mCuda = NULL;

	// vector para identificar entidades
	std::vector<int> _ids;
	std::vector<int> ids_erase;
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
