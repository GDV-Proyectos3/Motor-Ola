#pragma once
#include "Componente.h"
#include "utils/Vectola3D.h"
#include "utils/Quaterniola.h"
#include <list>

class Transform :   public Componente
{
public:
	// OBLIGATORIO EN CADA COMPONENTE
	// Constructor sin parámetros
	Transform();
	// Destructor
	virtual ~Transform();
	// Función para inicializar el componente mediante datos serializados
	bool init(const std::map<std::string, std::string>& mapa);

	// Movimientos
	enum class Space { Self, World,	Parent };
	void translate(double x, double y, double z);
	void rotate(float xAngle, float yAngle, float zAngle, Space relativeTo = Space::Self);

	// Getters
	Transform* getParent() { return _parent; }
	Transform* findChild(char* name);

	Vectola3D getPosition() { getParentData(); return _position; }
	Quaterniola getRotation() { getParentData(); return _rotation; }
	Vectola3D getScale() { getParentData(); return _scale; }

	Vectola3D getLocalPosition() { return _localPosition; }
	Quaterniola getLocalRotation() { return _localRotation; }
	Vectola3D getLocalScale() { return _localScale; }

	// Setter
	void setParent(Transform* par);

	void setPosition(Vectola3D v);
	void setPosition(float x, float y, float z);

	void setRotation(Quaterniola q);
	Quaterniola inverseTransformRotation(Quaterniola q);
	void setRotation(float x, float y, float z);

	void setScale(Vectola3D v);
	void setScale(float x, float y, float z);

	void setLocalPosition(Vectola3D v);
	void setLocalPosition(float x, float y, float z);

	void setLocalRotation(Quaterniola q);
	Quaterniola transformRotation(Quaterniola q);
	void setLocalRotation(float x, float y, float z);

	void setLocalScale(Vectola3D v);
	void setLocalScale(float x, float y, float z);


	// Transforma el vector direction del espacio local al espacio global
	Vectola3D transformDirection(Vectola3D direction);
	// Transforma la posicion x, y, z del espacio local al espacio global
	Vectola3D transformDirection(float x, float y, float z);

	// Transforma el vector direction del espacio global al espacio local
	Vectola3D inverseTransformDirection(Vectola3D direction);
	// Transforma la posicion x, y, z del espacio global al espacio local
	Vectola3D inverseTransformDirection(float x, float y, float z);

private:
	Transform* _parent = nullptr;
	std::list<Transform*> _children;

	// Añade a un hijo a la lista
	void setChild(Transform* child) { _children.push_back(child); }
	// Elimina a un hijo de la lista
	void removeChild(Transform* child) { if (child->_parent == this) _children.remove(child); };
	// Actualiza los datos en funcion del padre
	void getParentData();

	// Posiciones globales
	Vectola3D _position;
	Quaterniola _rotation;
	Vectola3D _scale;	
	
	// Posiciones locales
	Vectola3D _localPosition;
	Quaterniola _localRotation;
	Vectola3D _localScale;
};

