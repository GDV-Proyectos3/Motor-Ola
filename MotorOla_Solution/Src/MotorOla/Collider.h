#pragma once

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
	PxShape* getShape() { return shape; };

private:
	PxShape* shape = nullptr;

	// forma geometrica por defecto
	PxGeometryType::Enum type_ = PxGeometryType::eBOX;
};