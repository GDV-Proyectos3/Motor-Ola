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
	PxVec3 getVelocity() { return vel_; };
	PxVec3 getAngularVelocity() { return body->getAngularVelocity(); };

	// Setter
	void setVelocity(PxVec3 v);
	void setAngularVelocity(PxVec3 av);

private:
	PxRigidDynamic* body = nullptr;
	PxRigidStatic* stBody = nullptr;

	// Parametros por defecto
	PxVec3 pos_ = PxVec3(PxZero);		// position
	PxQuat ori_ = PxQuat(PxIdentity);	// orientation
	PxVec3 vel_ = PxVec3(PxZero);		// velocity

	bool no1stTry = true;
};
