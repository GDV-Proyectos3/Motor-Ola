#pragma once
#include <string>
#include <unordered_map>
#include "utils/Singleton.h"
class Componente;

typedef Componente* (*componentInstanceGenerator) ();

class ComponenteFactoria : public Singleton<ComponenteFactoria> {
public:
	///<summary>
	///Devuele el componente que quieres si esta guardado en mGenerators, si no existe devuelve nullptr
	///</summary>
	Componente* getComponent(std::string name);

	///<summary>
	///Registra el nuevo componente que le pasas, primero el nombre (identificador del ecs) y luego el componente como tal
	///</summary>
	bool registerGenerator(std::string compName, const componentInstanceGenerator& instGenerator);

private:
	ComponenteFactoria() {};
	~ComponenteFactoria() {};
	///<summary>
	// Tabla hash donde guardamos todos los componentes, string es el nombre
	// componentInstanceGenerator es el componente como tal
	///</summary>
	std::unordered_map<std::string, componentInstanceGenerator> _mGenerators;
};