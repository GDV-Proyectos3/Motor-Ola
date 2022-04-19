#pragma once
#include<vector>
#include <memory>

class Entidad;

class EntidadManager {
	using uptr_ent = std::unique_ptr<Entidad>;
public:
	EntidadManager() {};
	virtual ~EntidadManager() {};


	Entidad* addEntidad();
	void update();
	void draw();
	void refresh();
	
	Entidad* getEntidadByID(int id);

	std::vector<uptr_ent> entitys_;
private:
};
