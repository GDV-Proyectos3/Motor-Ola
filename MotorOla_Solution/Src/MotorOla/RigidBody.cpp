#include "RigidBody.h"
#include "PhysxManager.h"

RigidBody::RigidBody() : PxRigidDynamic(concreteType_, baseFlags_)
{}

RigidBody::~RigidBody()
{
}

bool RigidBody::init(const std::map<std::string, std::string>& mapa)
{
	// comprobar que la sección existe
	if (mapa.find("posX") == mapa.end() ||
		mapa.find("posY") == mapa.end() ||
		mapa.find("posZ") == mapa.end() ||
		mapa.find("oriX") == mapa.end() ||
		mapa.find("oriY") == mapa.end() ||
		mapa.find("oriZ") == mapa.end() ||
		mapa.find("oriW") == mapa.end())
	{
		///return false;///
		
		// Position and orientation in world space (Physx pose)
		PxTransform pose = PxTransform(position_, orientation_);
		physX()->createRigidDynamic(pose);

		return true;
	}

	// traducción
	std::string pxString = mapa.at("posX");
	float positionX = stof(pxString);
	std::string pyString = mapa.at("posY");
	float positionY = stof(pyString);
	std::string pzString = mapa.at("posZ");
	float positionZ = stof(pzString);
	///
	std::string oxString = mapa.at("oriX");
	float orientationX = stof(oxString);
	std::string oyString = mapa.at("oriY");
	float orientationY = stof(oyString);
	std::string ozString = mapa.at("oriZ");
	float orientationZ = stof(ozString);
	std::string owString = mapa.at("oriW");
	float orientationW = stof(owString);

	// establecer propiedades
	position_ = PxVec3(positionX, positionY, positionZ);
	orientation_ = PxQuat(orientationX, orientationY, orientationZ, orientationW);

	// Position and orientation in world space (Physx pose)
	PxTransform pose = PxTransform(position_, orientation_);
	physX()->createRigidDynamic(pose);

	return true;
}