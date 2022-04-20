#pragma once
#include "utils/Vectola3D.h"
//#include "Matrix3.h"
//#include "LinearMath/btQuaternion.h"

class Quaternion {
public:
	float s;
	Vectola3D v;

	// Constructores
	Quaternion();
	Quaternion(float w, float x, float y, float z);
	Quaternion(float scalar, Vectola3D& vector);
	Quaternion(const Quaternion& quat);

	// Destructor
	~Quaternion() {};

	/// <summary>
	/// Convierte el vector dado a Quaternion
	/// </summary>
	/// <param name="vector"> Vector a convertir </param>
	/// <returns></returns>
	static Quaternion Euler(Vectola3D vector);

	static Quaternion Lerp(Quaternion a, Quaternion b, float t);

	static Quaternion Slerp(Quaternion a, Quaternion b, float t, float threshold);

	/// <summary>
	/// Devuelve el angulo entre dos Quaterniones
	/// </summary>
	/// <returns></returns>
	static float Angle(Quaternion& a, Quaternion& b);


	float magnitude();
	Quaternion normalize();
	// Invierte el signo de la parte imaginaria, actua como inversa
	Quaternion conjugate();
	// La inversa, pero normalizada, es un metodo un poco redundante pues solo se 
	// tendria que usar si se han puesto los Quaterniones a mano y no mediante Euler()
	Quaternion inverse();
	/// <summary>
	/// Convierte el Quaternion en angulos de Euler.
	/// </summary>
	/// <returns></returns>
	Vectola3D toEuler();
	/// <summary>
	///	Convierte el Quaternion en un vector unitario que apunta en su direccion.
	/// </summary>
	/// <returns></returns>
	//Vectola3D toVector();

	float dotProduct(const Quaternion& q);

	Quaternion& operator=(const Quaternion& quat);
	void operator+=(const Quaternion& quat);
	Quaternion operator+(const Quaternion& quat) const;
	void operator-=(const Quaternion& quat);
	Quaternion operator-(const Quaternion& quat) const;
	void operator*=(const Quaternion& quat);
	Quaternion operator*(const Quaternion& quat) const;
	void operator*=(const float value);
	Quaternion operator*(const float value) const;
	void operator*=(const Vectola3D value);
	Vectola3D operator*(const Vectola3D value) const;
	bool operator==(const Quaternion& quat) const;


#ifdef _DEBUG
	friend std::ostream& operator<<(std::ostream& output, const Quaternion& q) {
		output << "(" << q.s << ", " << q.v.getX() << ", " << q.v.getY() << ", " << q.v.getZ() << ")" << "\n";
		return output;
	};
#endif // _DEBUG

	operator Ogre::Quaternion() const;

};

