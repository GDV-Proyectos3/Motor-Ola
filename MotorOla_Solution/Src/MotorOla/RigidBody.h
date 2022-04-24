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
	RigidBody(PxType concreteType, PxBaseFlags baseFlags) : PxRigidDynamic(concreteType, baseFlags) {};
	// Destructor
	virtual ~RigidBody();
	// Funci�n para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa);

	// Getters

	// Setters

	//...

private:

	//...
};