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
	// Constructor sin par�metros
	/**
	* PxType: 
	* PxBaseFlags: ( eOWNS_MEMORY | eIS_RELEASABLE ) posee memoria o es liberable.
	*/
	Collider(PxType concreteType = PxConcreteType::eSHAPE, 
		PxBaseFlags baseFlags = PxBaseFlag::eOWNS_MEMORY,
		PxGeometryType::Enum type = PxGeometryType::eBOX
	);
	// Destructor
	virtual ~Collider();
	// Funci�n para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa);

	// Getters

	// Setters

	//...

private:

	//...
};