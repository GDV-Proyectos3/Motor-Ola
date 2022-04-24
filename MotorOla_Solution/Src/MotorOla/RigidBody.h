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
	/**
	* PxType: 
	* PxBaseFlags: ( eOWNS_MEMORY | eIS_RELEASABLE ) posee memoria o es liberable.
	*/
	RigidBody(PxType concreteType = PxConcreteType::eRIGID_DYNAMIC, 
		PxBaseFlags baseFlags = PxBaseFlag::eOWNS_MEMORY, 
		PxVec3 position = PxVec3(PxZero), 
		PxQuat orientation = PxQuat(PxIdentity)
	);
	// Destructor
	virtual ~RigidBody();
	// Función para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa);

	// Getters

	// Setters

	//...

private:
	//...
};