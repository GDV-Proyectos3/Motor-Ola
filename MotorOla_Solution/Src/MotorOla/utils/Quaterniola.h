#pragma once
#include "utils/Vectola3D.h"
//#include "Matrix3.h"
//#include "LinearMath/btQuaternion.h"

class Quaterniola {
public:
	float s;
	Vectola3D v;

	// Constructores
	Quaterniola();
	Quaterniola(float w, float x, float y, float z);
	Quaterniola(float scalar, Vectola3D& vector);
	Quaterniola(const Quaterniola& quat);

	// Destructor
	~Quaterniola() {};

	/// <summary>
	/// Convierte el vector dado a Quaternion
	/// </summary>
	/// <param name="vector"> Vector a convertir </param>
	/// <returns></returns>
	static Quaterniola Euler(Vectola3D vector);

	static Quaterniola Lerp(Quaterniola a, Quaterniola b, float t);

	static Quaterniola Slerp(Quaterniola a, Quaterniola b, float t, float threshold);

	/// <summary>
	/// Devuelve el angulo entre dos Quaterniones
	/// </summary>
	/// <returns></returns>
	static float Angle(Quaterniola& a, Quaterniola& b);


	float magnitude();
	Quaterniola normalize();
	// Invierte el signo de la parte imaginaria, actua como inversa
	Quaterniola conjugate();
	// La inversa, pero normalizada, es un metodo un poco redundante pues solo se 
	// tendria que usar si se han puesto los Quaterniones a mano y no mediante Euler()
	Quaterniola inverse();
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

	float dotProduct(const Quaterniola& q);

	Quaterniola& operator=(const Quaterniola& quat);
	void operator+=(const Quaterniola& quat);
	Quaterniola operator+(const Quaterniola& quat) const;
	void operator-=(const Quaterniola& quat);
	Quaterniola operator-(const Quaterniola& quat) const;
	void operator*=(const Quaterniola& quat);
	Quaterniola operator*(const Quaterniola& quat) const;
	void operator*=(const float value);
	Quaterniola operator*(const float value) const;
	void operator*=(const Vectola3D value);
	Vectola3D operator*(const Vectola3D value) const;
	bool operator==(const Quaterniola& quat) const;


#ifdef _DEBUG
	friend std::ostream& operator<<(std::ostream& output, const Quaterniola& q) {
		output << "(" << q.s << ", " << q.v.getX() << ", " << q.v.getY() << ", " << q.v.getZ() << ")" << "\n";
		return output;
	};
#endif // _DEBUG

	operator Ogre::Quaternion() const;

};

