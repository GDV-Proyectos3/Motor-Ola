#pragma once
#pragma once

#include "PxRigidDynamic.h"
#include "Componente.h"
///#include "utils/Vectola3D.h"
///#include "utils/Quaterniola.h"
///#include <list>

using namespace physx;

class RigidBody : public Componente, public PxRigidDynamic
{
public:
	// OBLIGATORIO EN CADA COMPONENTE
	// Constructor sin par�metros 
	RigidBody();
	// Destructor
	virtual ~RigidBody();
	// Funci�n para inicializar el componente mediante datos serializados
	bool init(PxVec3 position, PxQuat orientation,
		const std::map<std::string, std::string>& mapa);

	// Getters

	// Setters

	//...

private:

	/**
	* PxType:
	* PxBaseFlags: ( eOWNS_MEMORY | eIS_RELEASABLE ) posee memoria o es liberable.
	*/
	PxType concreteType_ = PxConcreteType::eRIGID_DYNAMIC;
	PxBaseFlags baseFlags_ = PxBaseFlag::eOWNS_MEMORY;

	PxVec3 position_ = PxVec3(PxZero); /// LUA
	PxQuat orientation_ = PxQuat(PxIdentity); /// LUA
};