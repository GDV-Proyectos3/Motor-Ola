#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif
#include "utils/Vectola3D.h"


class Quaterniola {
public:
	float s;
	Vectola3D v;

	// Constructores
	MOTOR_API Quaterniola();
	MOTOR_API Quaterniola(float w, float x, float y, float z);
	MOTOR_API Quaterniola(float scalar, Vectola3D& vector);
	MOTOR_API Quaterniola(const Quaterniola& quat);

	// Destructor
	MOTOR_API ~Quaterniola() {};

	/// <summary>
	/// Convierte el vector dado a Quaternion
	/// </summary>
	/// <param name="vector"> Vector a convertir </param>
	/// <returns></returns>
	MOTOR_API static Quaterniola Euler(Vectola3D vector);

	MOTOR_API static Quaterniola Lerp(Quaterniola a, Quaterniola b, float t);

	MOTOR_API Quaterniola Slerp(Quaterniola a, Quaterniola b, float t, float threshold);

	/// <summary>
	/// Devuelve el angulo entre dos Quaterniones
	/// </summary>
	/// <returns></returns>
	MOTOR_API static float Angle(Quaterniola& a, Quaterniola& b);


	MOTOR_API float magnitude();
	MOTOR_API Quaterniola normalize();
	// Invierte el signo de la parte imaginaria, actua como inversa
	MOTOR_API Quaterniola conjugate();
	// La inversa, pero normalizada, es un metodo un poco redundante pues solo se 
	// tendria que usar si se han puesto los Quaterniones a mano y no mediante Euler()
	MOTOR_API Quaterniola inverse();
	/// <summary>
	/// Convierte el Quaternion en angulos de Euler.
	/// </summary>
	/// <returns></returns>
	MOTOR_API Vectola3D toEuler();
	/// <summary>
	///	Convierte el Quaternion en un vector unitario que apunta en su direccion.
	/// </summary>
	/// <returns></returns>

	MOTOR_API float dotProduct(const Quaterniola& q);

	MOTOR_API Quaterniola& operator=(const Quaterniola& quat);
	MOTOR_API void operator+=(const Quaterniola& quat);
	MOTOR_API Quaterniola operator+(const Quaterniola& quat) const;
	MOTOR_API void operator-=(const Quaterniola& quat);
	MOTOR_API Quaterniola operator-(const Quaterniola& quat) const;
	MOTOR_API void operator*=(const Quaterniola& quat);
	MOTOR_API Quaterniola operator*(const Quaterniola& quat) const;
	MOTOR_API void operator*=(const float value);
	MOTOR_API Quaterniola operator*(const float value) const;
	MOTOR_API void operator*=(const Vectola3D value);
	MOTOR_API Vectola3D operator*(const Vectola3D value) const;
	MOTOR_API bool operator==(const Quaterniola& quat) const;


#ifdef _DEBUG
	friend std::ostream& operator<<(std::ostream& output, const Quaterniola& q) {
		output << "(" << q.s << ", " << q.v.getX() << ", " << q.v.getY() << ", " << q.v.getZ() << ")" << "\n";
		return output;
	};
#endif // _DEBUG

	operator Ogre::Quaternion() const;

};

