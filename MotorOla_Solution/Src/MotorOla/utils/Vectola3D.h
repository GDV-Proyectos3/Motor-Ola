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
	double x_;
	double y_;
	double z_;

public:
	// Constructores
	MOTOR_API Vectola3D();
	MOTOR_API Vectola3D(const Vectola3D&v);
	MOTOR_API Vectola3D(double x, double y, double z);

	MOTOR_API virtual ~Vectola3D();

	MOTOR_API inline double getX() const { return x_; }
	MOTOR_API inline double getY() const { return y_; }
	MOTOR_API inline double getZ() const { return z_; }

	MOTOR_API inline void setX(double x) { x_ = x; }
	MOTOR_API inline void setY(double y) { y_ = y; }
	MOTOR_API inline void setZ(double z) { z_ = z; }

	MOTOR_API inline void set(const Vectola3D& v) { x_ = v.x_; y_ = v.y_; z_ = v.z_; }
	MOTOR_API inline void set(double x, double y, double z) { x_ = x;	y_ = y;	z_ = z; }
	 
	MOTOR_API inline double magnitude() const { return sqrt(pow(x_, 2) + pow(y_, 2) + pow(z_, 2)); }

	// Funciones sin refactorizar a 3d
	MOTOR_API Vectola3D rotate(double degrees) const;
	MOTOR_API double angle(const Vectola3D& v) const;
	MOTOR_API Vectola3D normalize() const;

	MOTOR_API double dotProduct(const Vectola3D& other) const { return x_ * other.x_ + y_ * other.y_ + z_ * other.z_; }

	MOTOR_API Vectola3D crossProduct(const Vectola3D& other) const {
		float nx = y_ * other.z_ - z_ * other.y_;
		float ny = z_ * other.x_ - x_ * other.z_;
		float nz = x_ * other.y_ - y_ * other.x_;
		return Vectola3D(nx, ny, nz);
	}

	MOTOR_API inline Vectola3D operator-(const Vectola3D& v) const { return Vectola3D(x_ - v.x_, y_ - v.y_, z_ - v.z_); }
	MOTOR_API inline Vectola3D operator+(const Vectola3D& v) const { return Vectola3D(x_ + v.x_, y_ + v.y_, z_ + v.z_); }
	MOTOR_API inline Vectola3D operator*(double d) const { return Vectola3D(x_ * d, y_ * d, z_ * d); }
	MOTOR_API inline double operator *(const Vectola3D& d) const { return d.x_ * x_ + d.y_ * y_ + d.z_ * z_; }
	MOTOR_API Vectola3D operator/(double d) const { return Vectola3D(x_ / d, y_ / d, z_ / d); }

	friend std::ostream& operator<<(std::ostream& os, const Vectola3D& v);

	MOTOR_API Ogre::Vector3 convertOgreVector(const Vectola3D& v)const;
};

