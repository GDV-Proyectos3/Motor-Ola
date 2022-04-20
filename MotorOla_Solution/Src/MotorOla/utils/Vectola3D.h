#pragma once

#include <iostream>
#include <math.h>
#include <Ogre.h>

class Vectola3D
{
	double x_;
	double y_;
	double z_;

public:
	// Constructores
	Vectola3D();
	Vectola3D(const Vectola3D&v);
	Vectola3D(double x, double y, double z);

	virtual ~Vectola3D();

	inline double getX() const { return x_; }
	inline double getY() const { return y_; }
	inline double getZ() const { return z_; }

	inline void setX(double x) { x_ = x; }
	inline void setY(double y) { y_ = y; }
	inline void setZ(double z) { z_ = z; }

	inline void set(const Vectola3D& v) { x_ = v.x_; y_ = v.y_; z_ = v.z_; }
	inline void set(double x, double y, double z) { x_ = x;	y_ = y;	z_ = z; }

	inline double magnitude() const { return sqrt(pow(x_, 2) + pow(y_, 2) + pow(z_, 2)); }

	// Funciones sin refactorizar a 3d
	Vectola3D rotate(double degrees) const;
	double angle(const Vectola3D& v) const;
	Vectola3D normalize() const;

	/// <summary>
/// Devuelve el producto escalar entre 2 vectores
/// </summary>
	double dotProduct(const Vectola3D& other) const { return x_ * other.x_ + y_ * other.y_ + z_ * other.z_; }

	/// <summary>
	/// Devuelve el producto cruzado entre 2 vectores
	/// </summary>
	Vectola3D crossProduct(const Vectola3D& other) const {
		float nx = y_ * other.z_ - z_ * other.y_;
		float ny = z_ * other.x_ - x_ * other.z_;
		float nz = x_ * other.y_ - y_ * other.x_;
		return Vectola3D(nx, ny, nz);
	}

	inline Vectola3D operator-(const Vectola3D& v) const { return Vectola3D(x_ - v.x_, y_ - v.y_, z_ - v.z_); }
	inline Vectola3D operator+(const Vectola3D& v) const { return Vectola3D(x_ + v.x_, y_ + v.y_, z_ + v.z_); }
	inline Vectola3D operator*(double d) const { return Vectola3D(x_ * d, y_ * d, z_ * d); }
	inline double operator *(const Vectola3D& d) const { return d.x_ * x_ + d.y_ * y_ + d.z_ * z_; }
	Vectola3D operator/(double d) const { return Vectola3D(x_ / d, y_ / d, z_ / d); }

	friend std::ostream& operator<<(std::ostream& os, const Vectola3D& v);

	Ogre::Vector3 convertOgreVector(const Vectola3D& v)const;
};

