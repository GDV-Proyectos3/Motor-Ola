#include "Transform.h"

Transform::Transform(Vectola3D position, Vectola3D scale, Vectola3D rotation) :
	_position(position), _scale(scale), _rotation(rotation)
{
}

Transform::Transform() :
	_position(), _scale(), _rotation()
{
}

Transform::~Transform()
{
}

void Transform::init()
{
}

void Transform::translate(double x, double y, double z)
{
	_position.setX(_position.getX() + x);
	_position.setY(_position.getY() + y);
	_position.setZ(_position.getZ() + z);
}

//2D,asi que solo puede rotar por eje Y
void Transform::rotate(double angle)
{
	//_rotation += angle;
	//double rot = abs(_rotation);
	//while (rot >= 360)
	//{
	//	rotation_ -= 360;
	//}
	//
	//if (rotation_ < 0)
	//	rot *= -1;
	//rotation_ = rot;
}
