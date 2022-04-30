#pragma once
#pragma once

#include "Componente.h"
#include "PxRigidDynamic.h"

using namespace physx;

class RigidBody : public Componente
{
public:
	// OBLIGATORIO EN CADA COMPONENTE
	// Constructor sin parámetros 
	RigidBody();
	// Destructor
	virtual ~RigidBody();
	// Función para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa);

	// Getter
	PxRigidDynamic* getBody() { return body; };
	PxRigidStatic* getStBody() { return stBody; };

private:
	PxRigidDynamic* body = nullptr;
	PxRigidStatic* stBody = nullptr;

	// parametros por defecto
	PxVec3 position_ = PxVec3(PxZero);
	PxQuat orientation_ = PxQuat(PxIdentity);
};