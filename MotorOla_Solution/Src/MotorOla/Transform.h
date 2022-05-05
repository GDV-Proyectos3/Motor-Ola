#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include "Componente.h"
#include "utils/Vectola3D.h"
#include "utils/Quaterniola.h"
#include <list>

class Transform : public Componente
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
	MOTOR_API enum class Space { Self, World, Parent };
	MOTOR_API void translate(double x, double y, double z);
	MOTOR_API void rotate(float xAngle, float yAngle, float zAngle, Space relativeTo = Space::Self);

	// Getters
	MOTOR_API Transform* getParent() { return _parent; }
	MOTOR_API Transform* findChild(char* name);

	MOTOR_API Vectola3D getPosition() { getParentData(); return _position; }
	MOTOR_API Quaterniola getRotation() { getParentData(); return _rotation; }
	MOTOR_API Vectola3D getScale() { getParentData(); return _scale; }

	MOTOR_API Vectola3D getLocalPosition() { return _localPosition; }
	MOTOR_API Quaterniola getLocalRotation() { return _localRotation; }
	MOTOR_API Vectola3D getLocalScale() { return _localScale; }

	// Setter
	MOTOR_API void setParent(Transform* par);

	MOTOR_API void setPosition(Vectola3D v);
	MOTOR_API void setPosition(float x, float y, float z);

	MOTOR_API void setRotation(Quaterniola q);
	MOTOR_API Quaterniola inverseTransformRotation(Quaterniola q);
	MOTOR_API void setRotation(float x, float y, float z);

	MOTOR_API void setScale(Vectola3D v);
	MOTOR_API void setScale(float x, float y, float z);

	MOTOR_API void setLocalPosition(Vectola3D v);
	MOTOR_API void setLocalPosition(float x, float y, float z);
	
	MOTOR_API void setLocalRotation(Quaterniola q);
	MOTOR_API Quaterniola transformRotation(Quaterniola q);
	MOTOR_API void setLocalRotation(float x, float y, float z);

	MOTOR_API void setLocalScale(Vectola3D v);
	MOTOR_API void setLocalScale(float x, float y, float z);


	// Transforma el vector direction del espacio local al espacio global
	MOTOR_API Vectola3D transformDirection(Vectola3D direction);
	// Transforma la posicion x, y, z del espacio local al espacio global
	MOTOR_API Vectola3D transformDirection(float x, float y, float z);

	// Transforma el vector direction del espacio global al espacio local
	MOTOR_API Vectola3D inverseTransformDirection(Vectola3D direction);
	// Transforma la posicion x, y, z del espacio global al espacio local
	MOTOR_API Vectola3D inverseTransformDirection(float x, float y, float z);

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