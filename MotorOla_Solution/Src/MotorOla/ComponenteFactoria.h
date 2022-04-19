#pragma once
#include <string>
#include <unordered_map>
#include "utils/Singleton.h"
class Componente;

typedef Componente* (*componentInstanceGenerator) ();

class ComponenteFactoria : public Singleton<ComponenteFactoria> {
public:
	///<summary>
	///Devuele el componente si esta guardado en mGenerators, si no existe devuelve nullptr
	///</summary>
	Componente* getComponent(std::string name);

	///<summary>
	///Registra el nuevo componente que le pasas, primero el nombre (identificador del ecs) y luego el componente como tal
	///</summary>
	bool registerGenerator(std::string compName, const componentInstanceGenerator& instGenerator);

	ComponenteFactoria() {};
	~ComponenteFactoria() {};
private:

	// Tabla hash donde guardamos todos los componentes | Nombre -> Componente
	std::unordered_map<std::string, componentInstanceGenerator> _mGenerators;
};