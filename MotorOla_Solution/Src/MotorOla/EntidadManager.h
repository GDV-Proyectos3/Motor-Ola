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
	

private:
	std::vector<uptr_ent> entitys_;
};
