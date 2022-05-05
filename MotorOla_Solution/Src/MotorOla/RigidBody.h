#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif 
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
	MOTOR_API PxRigidDynamic* getBody() { return body; };
	MOTOR_API PxRigidStatic* getStBody() { return stBody; };
	MOTOR_API PxVec3 getVelocity() { return _vel; };
	MOTOR_API PxVec3 getAngularVelocity() { return body->getAngularVelocity(); };

	// Setter
	MOTOR_API void setVelocity(PxVec3 v);
	MOTOR_API void setAngularVelocity(PxVec3 av);

private:
	PxRigidDynamic* body = nullptr;
	PxRigidStatic* stBody = nullptr;

	// Parametros por defecto
	PxVec3 _pos = PxVec3(PxZero);		// position
	PxQuat _ori = PxQuat(PxIdentity);	// orientation
	PxVec3 _vel = PxVec3(PxZero);		// velocity
};
