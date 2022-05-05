#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include <vector>
#include <memory>
#include <string>
#include <stack>

#include "utils\Singleton.h"
class Entidad;


MOTOR_API class EntidadManager : public Singleton<EntidadManager> {
	friend Singleton<EntidadManager>;

	using uptr_ent = std::unique_ptr<Entidad>;
public:
	EntidadManager() {};
	virtual ~EntidadManager() {};

	MOTOR_API Entidad* addEntidad();
	MOTOR_API Entidad* addEntidad(std::string entityName, int id);
	MOTOR_API void update();
	MOTOR_API void draw();
	MOTOR_API void refresh();
	MOTOR_API void pauseEntidades();
	MOTOR_API void reanudeEntidades();
	MOTOR_API void sincronizaVectorEnt();
	
	MOTOR_API std::vector<uptr_ent>& getAllEntidades() {
		return _entitys;
	}
	MOTOR_API Entidad* getEntidadByID(int id);

private:
	std::vector<uptr_ent> _entitys;
};

// Esta macro define una forma compacta para usar el Singleton EntidadManager, 
// en lugar de escribir 'EntidadManager::instance()->method()' escribiremos 'em().method()'
inline EntidadManager& em() {
	return *EntidadManager::instance();
}