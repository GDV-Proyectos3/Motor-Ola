#pragma once
#include <vector>
#include <memory>
#include <string>
#include <stack>

#include "utils\Singleton.h"
class Entidad;


class EntidadManager : public Singleton<EntidadManager> {
	friend Singleton<EntidadManager>;

	using uptr_ent = std::unique_ptr<Entidad>;
public:
	EntidadManager() {};
	virtual ~EntidadManager() {};

//static std::unique_ptr<EntidadManager> Singleton<EntidadManager>::instance_;

	Entidad* addEntidad();
	Entidad* addEntidad(std::string entityName, int id);
	void update();
	void draw();
	void refresh();
	void pauseEntidades();
	void reanudeEntidades();
	
	Entidad* getEntidadByID(int id);

private:
	std::vector<uptr_ent> entitys_;

	// Idea para guardar las entidades al poner nuevas escenas
	//std::stack <std::vector<uptr_ent>> pilaEntidades_;
};
