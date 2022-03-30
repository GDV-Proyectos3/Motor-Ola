#pragma once
#include "Componente.h"
#include "./utils/Vectola3D.h"

class Transform :   public Componente
{
public:
	Transform(Vectola3D position, Vectola3D scale, Vectola3D rotation);
	Transform();
	virtual ~Transform();

	void init();
	void translate(double x, double y, double z);
	void rotate(double angle);

	// Funciones con posicion
	inline const Vectola3D& getPos() const { return _position; }
	inline void setPos(const Vectola3D& pos) { _position.set(pos); }
	inline void setPos(double x, double y, double z) { _position.set(x, y, z); }
	inline void setPosX(double x) { _position.setX(x); }
	inline void setPosY(double y) { _position.setY(y); }
	inline void setPosZ(double z) { _position.setZ(z); }

	// Funciones con rotation
	inline const Vectola3D& getRot() const { return _rotation; }
	inline void setRot(const Vectola3D& rot) { _rotation.set(rot); }
	inline void setRot(double x, double y, double z) { _rotation.set(x, y, z); }
	inline void setRotX(double x) { _rotation.setX(x); }
	inline void setRotY(double y) { _rotation.setY(y); }
	inline void setRotZ(double z) { _rotation.setZ(z); }

	// Funciones con scale
	inline const Vectola3D& getScale() const { return _scale; }
	inline void setScale(const Vectola3D& scale) { _scale.set(scale); }
	inline void setScale(double x, double y, double z) { _scale.set(x, y, z); }
	inline void setScaleX(double x) { _scale.setX(x); }
	inline void setScaleY(double y) { _scale.setY(y); }
	inline void setScaleZ(double z) { _scale.setZ(z); }

private:
	Vectola3D _position;
	Vectola3D _rotation;
	Vectola3D _scale;
};

