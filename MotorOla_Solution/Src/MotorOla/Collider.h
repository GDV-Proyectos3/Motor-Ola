#pragma once

#include "PxShape.h"
#include "Componente.h"
///#include "utils/Vectola3D.h"
///#include "utils/Quaterniola.h"
///#include <list>

class Collider : public Componente, public PxShape
{
public:
	// OBLIGATORIO EN CADA COMPONENTE
	// Constructor sin parámetros
	/**
	* PxType: 
	* PxBaseFlags: ( eOWNS_MEMORY | eIS_RELEASABLE ) posee memoria o es liberable.
	*/
	Collider(PxType concreteType, PxBaseFlags baseFlags) : PxShape(concreteType, baseFlags) {};
	// Destructor
	virtual ~Collider();
	// Función para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa);

	// Getters

	// Setters

	//...

private:

	//...
};