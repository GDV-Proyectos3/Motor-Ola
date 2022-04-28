#include "RigidBody.h"
#include "PhysxManager.h"

RigidBody::RigidBody() : PxRigidDynamic(concreteType_, baseFlags_)
{}

RigidBody::~RigidBody()
{
}

bool RigidBody::init(PxVec3 position, PxQuat orientation,
	const std::map<std::string, std::string>& mapa)
{
	position_ = position;
	orientation_ = orientation;

	// Position and orientation in world space (Physx pose)
	PxTransform pose = PxTransform(position_, orientation_);
	physX()->createRigidDynamic(pose);

	// TODO: lectura del archivo .lua
	return false;
}