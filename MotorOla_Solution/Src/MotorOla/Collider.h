#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif
#include "Componente.h"
#include "PxShape.h"

using namespace physx;

class Collider : public Componente
{
public:
	// OBLIGATORIO EN CADA COMPONENTE
	// Constructor sin parámetros
	Collider();
	// Destructor
	virtual ~Collider();
	// Función para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa);

	// Getter
	MOTOR_API PxShape* getShape() { return shape; };

private:
	PxShape* shape = nullptr;

	// forma geometrica por defecto
	PxGeometryType::Enum _type = PxGeometryType::eBOX;
};