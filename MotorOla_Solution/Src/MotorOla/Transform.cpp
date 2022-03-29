#include "Transform.h"

Transform::Transform(Vector2D pos, Vector2D vel, double width, double height, double rotation):
	position_(pos),
	velocity_(vel), 
	width_(width), 
	height_(height), 
	rotation_(rotation)
{
}

Transform::Transform():
	position_(),
	velocity_(),
	width_(1),
	height_(1),
	rotation_()
{
}

Transform::~Transform()
{
}

void Transform::init()
{
}

void Transform::translate(double x, double y)
{
	position_.setX(position_.getX() + x);
	position_.setY(position_.getY() + y);
}

//2D,asi que solo puede rotar por eje Y
void Transform::rotate(double angle)
{
	rotation_ += angle;
	double rot = abs(rotation_);
	while (rot >= 360)
	{
		rotation_ -= 360;
	}
	
	if (rotation_ < 0)
		rot *= -1;
	rotation_ = rot;
	

}
