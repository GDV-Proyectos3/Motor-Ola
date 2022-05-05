#include "utils/Quaterniola.h"

// needed for visual studio
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

Quaterniola::Quaterniola() : s(1), v(0, 0, 0) {};

Quaterniola::Quaterniola(float w, float x, float y, float z) : s(w), v(x, y, z) {}

Quaterniola::Quaterniola(float scalar, Vectola3D& vector) : s(scalar), v(vector) {}

Quaterniola::Quaterniola(const Quaterniola& quat) : s(quat.s), v(quat.v) {}

Quaterniola Quaterniola::Euler(Vectola3D vector) {
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

Quaterniola Quaterniola::Lerp(Quaterniola a, Quaterniola b, float t) {
	if (t <= 0) return a;
	else if (t >= 1) return b;
	float scale = 1 - t;
	return (a * scale + b * t);
}

Quaterniola Quaterniola::Slerp(Quaterniola a, Quaterniola b, float t, float threshold) {
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
		return Quaterniola::Lerp(a, b, t);
}

float Quaterniola::Angle(Quaterniola& a, Quaterniola& b) {
	Quaterniola ab = a.conjugate() * b;
	return 2.0 * atan2(ab.v.magnitude(), ab.s);
}

float Quaterniola::magnitude() {
	float scalar = s * s;
	float vec = v.dotProduct(v);
	return sqrt(scalar + vec);
}

Quaterniola Quaterniola::normalize() {
	float mag = magnitude();
	if (mag != 0) {
		float normalizeValue = 1 / mag;

		s *= normalizeValue;
		v = v * normalizeValue;
		return *this;
	}

	throw std::domain_error("Magnitude equals zero");
}

Quaterniola Quaterniola::conjugate() {
	Vectola3D vec = v * -1;
	return { s, vec };
}

Quaterniola Quaterniola::inverse() {
	float mag = magnitude();
	mag *= mag;
	mag = 1 / mag;

	Quaterniola conj = conjugate();
	Vectola3D vec = conj.v * mag;

	return { conj.s * mag , vec };
}

Vectola3D Quaterniola::toEuler() {
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



float Quaterniola::dotProduct(const Quaterniola& q)
{
	return(v.getX() * q.v.getX()) + (v.getY() * q.v.getY()) + (v.getZ() * q.v.getZ()) + (s * q.s);
}

Quaterniola& Quaterniola::operator=(const Quaterniola& quat) {
	s = quat.s;
	v = quat.v;
	return *this;
}

void Quaterniola::operator+=(const Quaterniola& quat) {
	s += quat.s;
	v = v + quat.v;
}

Quaterniola Quaterniola::operator+(const Quaterniola& quat) const {
	Vectola3D vec = v + quat.v;
	return { s + quat.s, vec };
}

void Quaterniola::operator-=(const Quaterniola& quat) {
	s -= quat.s;
	v = v - quat.v;
}

Quaterniola Quaterniola::operator-(const Quaterniola& quat) const {
	Vectola3D vec = v - quat.v;
	return { s - quat.s, vec };
}

void Quaterniola::operator*=(const Quaterniola& quat) {
	s = s * quat.s - v.dotProduct(quat.v);
	v = quat.v * s + v * quat.s + v.crossProduct(quat.v);
}

Quaterniola Quaterniola::operator*(const Quaterniola& quat) const {
	float scalar = s * quat.s - v.dotProduct(quat.v);
	Vectola3D vec = quat.v * s + v * quat.s + v.crossProduct(quat.v);
	return { scalar, vec };
}

void Quaterniola::operator*=(const float value) {
	s *= value;
	v = v * value;
}

Quaterniola Quaterniola::operator*(const float value) const {
	Vectola3D vec = v * value;
	return { s * value, vec };
}

Vectola3D Quaterniola::operator*(const Vectola3D value) const
{
	Vectola3D v1, v2;
	v1 = v.crossProduct(value);
	v2 = v.crossProduct(v1);
	v1 = v1 * (2.0f * s);
	v2 = v1 * 2.0f;
	return v1 + v2 + v;
}

bool Quaterniola::operator==(const Quaterniola& value) const {
	return s == value.s && (v.getX() == value.v.getX() && v.getY() == value.v.getY() && v.getZ() == value.v.getZ());
}

Quaterniola::operator Ogre::Quaternion() const {
	return { s, (float)v.getX(), (float)v.getY(), (float)v.getZ() };
}