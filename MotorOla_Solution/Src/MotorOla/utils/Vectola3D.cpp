#include "Vectola3D.h"
#include <assert.h>

// needed for visual studio
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

Vectola3D::Vectola3D() :
	_x(), _y(), _z() {
}

Vectola3D::Vectola3D(const Vectola3D& v) :
	_x(v.getX()), _y(v.getY()), _z(v.getZ()) {
}

Vectola3D::Vectola3D(double x, double y, double z) :
	_x(x), _y(y), _z(z) {
}

Vectola3D::~Vectola3D() {
}

// TO DO
Vectola3D Vectola3D::rotate(double degrees) const {
	Vectola3D r;

	//degrees = fmod(degrees, 360.0);
	//if (degrees > 180.0) {
	//	degrees = degrees - 360.0;
	//}
	//else if (degrees <= -180.0) {
	//	degrees = 360.0 + degrees;
	//}

	//assert(degrees >= -180.0 && degrees <= 180.0);

	//double angle = degrees * M_PI / 180.0;
	//double sine = sin(angle);
	//double cosine = cos(angle);

	////rotation matix
	//double matrix[2][2] = { { cosine, -sine }, { sine, cosine } };

	//double x = _x;
	//double y = _y;

	//r._x = matrix[0][0] * x + matrix[0][1] * y;
	//r._y = matrix[1][0] * x + matrix[1][1] * y;

	return r;

}

// TO DO
double Vectola3D::angle(const Vectola3D& v) const {

	double a2 = atan2(v.getX(), v.getY());
	double a1 = atan2(_x, _y);
	//double sign = a1 > a2 ? 1 : -1;
	double angle = a1 - a2;
	//double K = -sign * M_PI * 2;
	//angle = (abs(K + angle) < abs(angle)) ? K + angle : angle;
	return angle * 180. / M_PI;
}

Vectola3D Vectola3D::normalize() const {
	Vectola3D r;
	r._x = _x;
	r._y = _y;
	r._z = _z;
	double mag = magnitude();
	if (mag > 0.0) {
		r._x = r._x / mag;
		r._y = r._y / mag;
		r._z = r._z / mag;
		
	}
	return r;
}

MOTOR_API bool Vectola3D::operator==(const Vectola3D& v) const
{
	return (_x == v.getX() && _y == v.getY() && _z == v.getZ());
}

Ogre::Vector3 Vectola3D::convertOgreVector(const Vectola3D& v) const
{
	return Ogre::Vector3(v.getX(), v.getX(), v.getZ());
}

std::ostream& operator<<(std::ostream& os, const Vectola3D& v) {
	os << "(" << v.getX() << "," << v.getY() << "," << v.getZ() << ")";
	return os;
}

