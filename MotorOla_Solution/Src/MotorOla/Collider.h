#pragma once

#include "PxShape.h"
#include "Componente.h"
///#include "utils/Vectola3D.h"
///#include "utils/Quaterniola.h"
///#include <list>

using namespace physx;

class Collider : public Componente, public PxShape
{
public:
	// OBLIGATORIO EN CADA COMPONENTE
	// Constructor sin parámetros
	Collider();
	// Destructor
	virtual ~Collider();
	// Función para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa);

	// Getters

	// Setters

	//...

private:

	/**
	* PxType:
	* PxBaseFlags: ( eOWNS_MEMORY | eIS_RELEASABLE ) posee memoria o es liberable.
	*/
	PxType concreteType_ = PxConcreteType::eSHAPE;
	PxBaseFlags baseFlags_ = PxBaseFlag::eOWNS_MEMORY;

	PxGeometryType::Enum type_ = PxGeometryType::eBOX;
};