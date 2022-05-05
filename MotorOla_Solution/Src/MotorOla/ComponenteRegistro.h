#pragma once

#include "ComponenteFactoria.h"
#include "ECS.h"
#include "utils/Singleton.h"

/// <summary>
/// Registro de los nuevos componentes, ejemplo:
/// ComponenteRegistro::ComponenteRegistro<Transform>("transform");
/// </summary>
namespace ComponenteRegistro {
	template<typename T> class ComponenteRegistro {
	public:
		ComponenteRegistro(std::string compName) {
			Singleton<ComponenteFactoria>::instance()->registerGenerator(compName, []()
				{
					return static_cast<Componente*>(new T());
				}
			);
		}
	};
}