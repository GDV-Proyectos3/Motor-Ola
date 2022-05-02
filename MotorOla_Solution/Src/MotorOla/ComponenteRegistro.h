#pragma once

//
//#ifdef MOTOR_EXPORTS
//#define MOTOR_API __declspec(dllexport)
//#else
//#define MOTOR_API __declspec(dllimport)
//#endif

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
			// Dejo esto comentado que no sé por que no funciona
			Singleton<ComponenteFactoria>::instance()->registerGenerator(compName, []()
				{
					return static_cast<Componente*>(new T());
				}
			);
		}
	};
}