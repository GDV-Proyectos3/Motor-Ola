#pragma once

#include "Componente.h"
///#include "utils/Vectola3D.h"
///#include "utils/Quaterniola.h"
///#include <list>

class Collider : public Componente
{
public:
	// OBLIGATORIO EN CADA COMPONENTE
	// Constructor sin par�metros
	Collider();
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