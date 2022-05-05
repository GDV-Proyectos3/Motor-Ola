#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include <iostream>
#include <math.h>
#include <Ogre.h>

MOTOR_API class Vectola3D
{
	double _x;
	double _y;
	double _z;

public:
	// Constructores
	MOTOR_API Vectola3D();
	MOTOR_API Vectola3D(const Vectola3D&v);
	MOTOR_API Vectola3D(double x, double y, double z);

	MOTOR_API virtual ~Vectola3D();

	MOTOR_API inline double getX() const { return _x; }
	MOTOR_API inline double getY() const { return _y; }
	MOTOR_API inline double getZ() const { return _z; }

	MOTOR_API inline void setX(double x) { _x = x; }
	MOTOR_API inline void setY(double y) { _y = y; }
	MOTOR_API inline void setZ(double z) { _z = z; }

	MOTOR_API inline void set(const Vectola3D& v) { _x = v._x; _y = v._y; _z = v._z; }
	MOTOR_API inline void set(double x, double y, double z) { _x = x;	_y = y;	_z = z; }
	 
	MOTOR_API inline double magnitude() const { return sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2)); }

	// Funciones sin refactorizar a 3d
	MOTOR_API Vectola3D rotate(double degrees) const;
	MOTOR_API double angle(const Vectola3D& v) const;
	MOTOR_API Vectola3D normalize() const;

	MOTOR_API double dotProduct(const Vectola3D& other) const { return _x * other._x + _y * other._y + _z * other._z; }

	MOTOR_API Vectola3D crossProduct(const Vectola3D& other) const {
		float nx = _y * other._z - _z * other._y;
		float ny = _z * other._x - _x * other._z;
		float nz = _x * other._y - _y * other._x;
		return Vectola3D(nx, ny, nz);
	}

	MOTOR_API bool operator==(const Vectola3D& quat) const;
	MOTOR_API inline Vectola3D operator-(const Vectola3D& v) const { return Vectola3D(_x - v._x, _y - v._y, _z - v._z); }
	MOTOR_API inline Vectola3D operator+(const Vectola3D& v) const { return Vectola3D(_x + v._x, _y + v._y, _z + v._z); }
	MOTOR_API inline Vectola3D operator*(double d) const { return Vectola3D(_x * d, _y * d, _z * d); }
	MOTOR_API inline double operator *(const Vectola3D& d) const { return d._x * _x + d._y * _y + d._z * _z; }
	MOTOR_API Vectola3D operator/(double d) const { return Vectola3D(_x / d, _y / d, _z / d); }

	friend std::ostream& operator<<(std::ostream& os, const Vectola3D& v);

	MOTOR_API Ogre::Vector3 convertOgreVector(const Vectola3D& v)const;
};

