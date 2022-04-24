#include "RigidBody.h"
#include "PhysxManager.h"

RigidBody::RigidBody(PxType concreteType, PxBaseFlags baseFlags, PxVec3 position, PxQuat orientation)
	: PxRigidDynamic(concreteType, baseFlags)
{
	// Position and orientation in world space (Physx pose)
	PxTransform pose = PxTransform(position, orientation);
	physx()->createRigidDynamic(pose);
}

RigidBody::~RigidBody()
{
}

bool RigidBody::init(const std::map<std::string, std::string>& mapa)
{
	// TODO: lectura del archivo .lua
	return false;
}