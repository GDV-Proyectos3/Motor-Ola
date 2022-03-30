#include "PhysxManager.h"

// _patata = interactive

PhysxManager::PhysxManager() : _patata(false) {
	PX_UNUSED(_patata);
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
}

PhysxManager::~PhysxManager()
{

}

void PhysxManager::update()
{

}

void PhysxManager::close()
{

}


