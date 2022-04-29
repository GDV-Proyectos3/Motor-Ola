#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include "utils\Singleton.h"
#include <PxPhysicsAPI.h>
#include <vector>
#include "Entidad.h"

using namespace physx;
//#include <string>

MOTOR_API class PhysxManager : public Singleton<PhysxManager> {
	friend Singleton<PhysxManager>;
	//using uptr_collider = std::unique_ptr<Px>;
public:	
	~PhysxManager();

	// Getters
	// ...
	MOTOR_API void init() {};
	MOTOR_API void update();
	MOTOR_API void close();

	MOTOR_API void createBall(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity/* = PxVec3(0)*/);
	MOTOR_API void attachBola(Entidad* ball);

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
