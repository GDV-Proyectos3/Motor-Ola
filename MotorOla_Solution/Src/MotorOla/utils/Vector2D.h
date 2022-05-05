#pragma once

#include <iostream>
#include <math.h>
#include <Ogre.h>

class Vector2D
{
	double _x;
	double _y;

public:
	Vector2D();
	Vector2D(const Vector2D &v);
	Vector2D(double x, double y);
	virtual ~Vector2D();
	inline double getX() const {
		return _x;
	}
	inline double getY() const {
		return _y;
	}
	inline void setX(double x) {
		_x = x;
	}
	inline void setY(double y) {
		_y = y;
	}
	inline void set(const Vector2D& v) {
		_x = v._x;
		_y = v._y;
	}
	inline void set(double x, double y) {
		_x = x;
		_y = y;
	}
	inline double magnitude() const {
		return sqrt(pow(_x, 2) + pow(_y, 2));
	}
	Vector2D rotate(double degrees) const;
	double angle(const Vector2D& v) const;
	Vector2D normalize() const;
	inline Vector2D operator-(const Vector2D& v) const {
		return Vector2D(_x - v._x, _y - v._y);
	}

	inline Vector2D operator+(const Vector2D& v) const {
		return Vector2D(_x + v._x, _y + v._y);
	}
	inline Vector2D operator*(double d) const {
		return Vector2D(_x * d, _y * d);
	}
	inline double operator *(const Vector2D& d) const {
		return d._x * _x + d._y * _y;
	}
	Vector2D operator/(double d) const {
		return Vector2D(_x / d, _y / d);
	}
	friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);

	Ogre::Vector3 convertOgreVector(const Vector2D& v)const;

};

