#pragma once
#include <vector>
#include <memory>
#include <string>

class Entidad;

class EntidadManager {
	using uptr_ent = std::unique_ptr<Entidad>;
public:
	EntidadManager() {};
	virtual ~EntidadManager() {};


	Entidad* addEntidad();
	Entidad* addEntidad(std::string entityName, int id);
	void update();
	void draw();
	void refresh();
	void pauseEntidades();
	void reanudeEntidades();
	
	Entidad* getEntidadByID(int id);

	std::vector<uptr_ent> entitys_;
private:
};
