#pragma once
#include "ComponenteFactoria.h"
#include "ECS.h"

/// <summary>
/// Registro de los nuevos componentes, ejemplo:
/// ComponenteRegistro::ComponenteRegistro<Transform>("transform");
/// </summary>
namespace ComponenteRegistro {
	template<typename T> class ComponenteRegistro {
	public:
		ComponenteRegistro(std::string compName) {
			ComponenteFactoria::getInstance().registerGenerator(compName, []()
				{
					return static_cast<Component*>(new T());
				}
			);
		}
	};
}