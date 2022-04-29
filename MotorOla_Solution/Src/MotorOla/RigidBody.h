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
	// Constructor sin parámetros 
	RigidBody();
	// Destructor
	virtual ~RigidBody();
	// Función para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa);

private:

	/**
	* PxType:
	* PxBaseFlags: ( eOWNS_MEMORY | eIS_RELEASABLE ) posee memoria o es liberable.
	*/
	PxType concreteType_ = PxConcreteType::eRIGID_DYNAMIC;
	PxBaseFlags baseFlags_ = PxBaseFlag::eOWNS_MEMORY;

	PxVec3 position_ = PxVec3(PxZero);
	PxQuat orientation_ = PxQuat(PxIdentity);
};