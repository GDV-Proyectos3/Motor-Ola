#pragma once
#include <vector>
#include "Componente.h"
#include "utils/Singleton.h"
#include "ComponenteFactoria.h"

class EntidadManager;
class Entidad
{
	using uptr_cmp = std::unique_ptr<Componente>;
public:
	Entidad();
	Entidad(std::string entityName, int id);

	void update();
	void draw() {};
	bool isActive() const;
	void destroy();

	virtual ~Entidad() {};

	inline EntidadManager* getEntityMngr() const { return entManager_; }

	inline void setEntityMngr(EntidadManager* mngr) { entManager_ = mngr; }

	inline int getID() { return _id; }

	template<typename  T>
	inline bool hasComponent() const {
		return componentBitset[getComponentTypeID<T>()];
	}

	template<typename T>
	inline T* getComponent() const {
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return static_cast<T*>(ptr);
	}

	//template<typename T, typename ... Targs>
	//inline T* addComponent(Targs&&...args) {

	//	T* c(new T(std::forward<Targs>(args)...));
	//	uptr_cmp uPtr(c);
	//	components.emplace_back(std::move(uPtr));

	//	componentArray[getComponentTypeID<T>()]=c;
	//	componentBitset[getComponentTypeID<T>()] = true;

	//	c->setEntidad(this);
	//	c->init();
	//	return c;
	//}

	Componente* addComponent(const std::string& compName, const std::map<std::string, std::string>& map) {
		Componente* t = Singleton<ComponenteFactoria>::instance()->getComponent(compName);
		if (t != nullptr) {
			t->entity_ = this;//ponemos la entidad en el componente
			std::unique_ptr<Componente> upt(t);
			components.push_back(std::move(upt));
			compMaps.push_back(map);
			compinits.push_back(false);

			return t;
		}
		throw std::exception("Error de carga del componente ");
	}

	inline void setActive(bool state) { active = state; }

	bool init();


private:
	std::string _name;
	int _id;

	EntidadManager* entManager_;

	std::vector<uptr_cmp> components;
	bool active = true;


	// Aqui estaran los componentes de esta entidad
	//std::vector<std::unique_ptr<Componente>> compUnique;
	std::vector<std::map<std::string, std::string>> compMaps;
	std::vector<bool> compinits;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;

	bool _needsOtherEntity = false;

	//numero de veces que puedes intentar iniciar tus componentes (para evitar bucle infinito)
	int _numTriesToLoad;
};

