#include "utils/Quaterniola.h"

// needed for visual studio
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

Quaternion::Quaternion() : s(1), v(0, 0, 0) {};

Quaternion::Quaternion(float w, float x, float y, float z) : s(w), v(x, y, z) {}

Quaternion::Quaternion(float scalar, Vectola3D& vector) : s(scalar), v(vector) {}

Quaternion::Quaternion(const Quaternion& quat) : s(quat.s), v(quat.v) {}

Quaternion Quaternion::Euler(Vectola3D vector) {
	Vectola3D v = vector * M_PI / 180;

	float cx = cosf(v.getX() * 0.5);
	float sx = sinf(v.getX() * 0.5);
	float cy = cosf(v.getY() * 0.5);
	float sy = sinf(v.getY() * 0.5);
	float cz = cosf(v.getZ() * 0.5);
	float sz = sinf(v.getZ() * 0.5);

	Vectola3D vec;
	float s = cx * cy * cz + sx * sy * sz;
	vec.setX(sx * cy * cz - cx * sy * sz);
	vec.setY(cx * sy * cz + sx * cy * sz);
	vec.setZ(cx * cy * sz - sx * sy * cz);

	float error = 0.000001;
	if (abs(s) < error) s = 0;
	if (abs(v.getX()) < error) v.setX(0);
	if (abs(v.getY()) < error) v.setY(0);
	if (abs(v.getZ()) < error) v.setZ(0);

	return { s, vec };
}

Quaternion Quaternion::Lerp(Quaternion a, Quaternion b, float t) {
	if (t <= 0) return a;
	else if (t >= 1) return b;
	float scale = 1 - t;
	return (a * scale + b * t);
}

Quaternion Quaternion::Slerp(Quaternion a, Quaternion b, float t, float threshold) {
	if (t <= 0) return a;
	else if (t >= 1) return b;
	float angle = a.dotProduct(b);

	// Aseguro la rotacion mas corta
	if (angle < 0.0f) {
		a *= -1.0f;
		angle *= -1.0f;
	}

	if (angle <= 1 - threshold) {
		float theta = acosf(angle);
		float invTheta = 1 / sinf(theta);
		float scale = sinf(theta * (1.0f - t)) * invTheta;
		float invScale = sinf(theta * t) * invTheta;

		return (a * scale + b * invScale);
	}
	else
		return Quaternion::Lerp(a, b, t);
}

float Quaternion::Angle(Quaternion& a, Quaternion& b) {
	Quaternion ab = a.conjugate() * b;
	return 2.0 * atan2(ab.v.magnitude(), ab.s);
}

float Quaternion::magnitude() {
	float scalar = s * s;
	float vec = v.dotProduct(v);
	return sqrt(scalar + vec);
}

Quaternion Quaternion::normalize() {
	float mag = magnitude();
	if (mag != 0) {
		float normalizeValue = 1 / mag;

		s *= normalizeValue;
		v = v * normalizeValue;
		return *this;
	}

	throw std::domain_error("Magnitude equals zero");
}

Quaternion Quaternion::conjugate() {
	Vectola3D vec = v * -1;
	return { s, vec };
}

Quaternion Quaternion::inverse() {
	float mag = magnitude();
	mag *= mag;
	mag = 1 / mag;

	Quaternion conj = conjugate();
	Vectola3D vec = conj.v * mag;

	return { conj.s * mag , vec };
}

Vectola3D Quaternion::toEuler() {
	Vectola3D vec;
	float sinx_cosy = 2 * (s * v.getX() + v.getY() * v.getZ());
	float cosx_cosy = 1 - 2 * (v.getX() * v.getX() + v.getY() * v.getY());
	vec.setX(atan2(sinx_cosy, cosx_cosy));

	float siny = 2 * (s * v.getY() - v.getZ() * v.getX());
	if (abs(siny) >= 1)
		vec.setY(copysignf((M_PI / 2), siny));
	else
		vec.setY(asinf(siny));

	float sinz_cosy = 2 * (s * v.getZ() + v.getX() * v.getY());
	float cosz_cosy = 1 - 2 * (v.getY() * v.getY() + v.getZ() * v.getZ());
	vec.setZ(atan2(sinz_cosy, cosz_cosy));

	float error = 0.000001;
	if (abs(vec.getX()) < error) vec.setX(0);
	if (abs(vec.getY()) < error) vec.setY(0);
	if (abs(vec.getZ()) < error) vec.setZ(0);

	return vec * 180 / M_PI;
}

//Vectola3D Quaternion::toVector()
//{
//	Matrix3 ang = toMatrix();
//	return { -ang.getColumn(2).x, -ang.getColumn(2).y, -ang.getColumn(2).z };;
//}

float Quaternion::dotProduct(const Quaternion& q)
{
	return(v.getX() * q.v.getX()) + (v.getY() * q.v.getY()) + (v.getZ() * q.v.getZ()) + (s * q.s);
}

Quaternion& Quaternion::operator=(const Quaternion& quat) {
	s = quat.s;
	v = quat.v;
	return *this;
}

void Quaternion::operator+=(const Quaternion& quat) {
	s += quat.s;
	v = v + quat.v;
}

Quaternion Quaternion::operator+(const Quaternion& quat) const {
	Vectola3D vec = v + quat.v;
	return { s + quat.s, vec };
}

void Quaternion::operator-=(const Quaternion& quat) {
	s -= quat.s;
	v = v - quat.v;
}

Quaternion Quaternion::operator-(const Quaternion& quat) const {
	Vectola3D vec = v - quat.v;
	return { s - quat.s, vec };
}

void Quaternion::operator*=(const Quaternion& quat) {
	s = s * quat.s - v.dotProduct(quat.v);
	v = quat.v * s + v * quat.s + v.crossProduct(quat.v);
}

Quaternion Quaternion::operator*(const Quaternion& quat) const {
	float scalar = s * quat.s - v.dotProduct(quat.v);
	Vectola3D vec = quat.v * s + v * quat.s + v.crossProduct(quat.v);
	return { scalar, vec };
}

void Quaternion::operator*=(const float value) {
	s *= value;
	v = v * value;
}

Quaternion Quaternion::operator*(const float value) const {
	Vectola3D vec = v * value;
	return { s * value, vec };
}

Vectola3D Quaternion::operator*(const Vectola3D value) const
{
	Vectola3D v1, v2;
	v1 = v.crossProduct(value);
	v2 = v.crossProduct(v1);
	v1 = v1 * (2.0f * s);
	v2 = v1 * 2.0f;
	return v1 + v2 + v;
}

bool Quaternion::operator==(const Quaternion& value) const {
	return s == value.s && (v.getX() == value.v.getX() && v.getY() == value.v.getY() && v.getZ() == value.v.getZ());
}

Quaternion::operator Ogre::Quaternion() const {
	return { s, (float)v.getX(), (float)v.getY(), (float)v.getZ() };
}