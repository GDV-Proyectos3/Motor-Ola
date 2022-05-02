#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif
#include <vector>
#include "Componente.h"
#include "utils/Singleton.h"
#include "ComponenteFactoria.h"
#include "utils/Vectola3D.h"
#include "utils/Quaterniola.h"

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

	void OnCollisionEnter(Entidad* other) { std::cout << "OnCollisionEnter\n"; };
	void OnTriggerEnter(Entidad* other) { std::cout << "OnTriggerEnter\n"; };

	virtual ~Entidad() {};

	inline EntidadManager* getEntityMngr() const { return entManager_; }

	MOTOR_API inline void setEntityMngr(EntidadManager* mngr) { entManager_ = mngr; }

	inline int getID() { return _id; }


	template<typename T>
	bool hasComponent() {
		return getComponent<T>() != nullptr;
	}

	/// <summary>
/// Devuelve un componente de la entidad, o nullptr si no lo tiene. Coste: O(N) :(
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns></returns>
	template<typename T>
	T* getComponent() {
		T* ret = nullptr;
		int i = 0;
		while (i < components.size() && ret == nullptr) {
			ret = dynamic_cast<T*>(components[i].get());
			++i;
		}
		return ret;
	}


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
	inline void setName(std::string n) { _name = n; }

	std::string getName() const { return _name; }

	bool init();

	MOTOR_API static Entidad* instantiate(std::string name, Vectola3D position = Vectola3D(), Quaterniola rotation = Quaterniola());

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
	int j;
};

