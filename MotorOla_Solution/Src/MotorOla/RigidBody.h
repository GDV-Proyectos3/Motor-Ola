#pragma once
#pragma once

#include "Componente.h"
///#include "utils/Vectola3D.h"
///#include "utils/Quaterniola.h"
///#include <list>

class RigidBody : public Componente
{
public:
	// OBLIGATORIO EN CADA COMPONENTE
	// Constructor sin par�metros
	RigidBody();
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