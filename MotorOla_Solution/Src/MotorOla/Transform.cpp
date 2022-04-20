#include "Transform.h"
#include "OgreManager.h"
#include "utils/Singleton.h"
#include "EntidadManager.h"
#include "Entidad.h"

Transform::Transform() :
	_position(), _scale({ 1,1,1 }), _rotation()
{
}

Transform::~Transform()
{
}

bool Transform::init(const std::map<std::string, std::string>& mapa)
{
	if (mapa.find("position") == mapa.end() || mapa.find("rotation") == mapa.end() || mapa.find("scale") == mapa.end() || mapa.find("parent") == mapa.end()) return false;
	std::string::size_type sz = 0, sa = 0;

	std::string parentString = mapa.at("parent");
	if (std::stoi(parentString) >= 0) {
		setParent(Singleton<EntidadManager>::instance()->getEntidadByID(stoi(parentString))->getComponent<Transform>());
	}
	if (_parent == nullptr) {
		_position = _localPosition;
		_rotation = _localRotation;
		_scale = _localScale;
	}

	std::string posString = mapa.at("position");
	float a = stof(posString, &sz);
	std::string temp = posString.substr(sz + 1);
	float b = stof(temp, &sa);
	float c = stof(posString.substr(sz + sa + 2));
	_position = { a, b, c };

	std::string s2 = mapa.at("rotation");
	_rotation = { stof(s2, &sz), stof(s2.substr(sz + 1), &sa), stof(s2.substr(sz + sa + 2)) };

	std::string s3 = mapa.at("scale");
	_scale = { stof(s3, &sz), stof(s3.substr(sz + 1), &sa), stof(s3.substr(sz + sa + 2)) };

	inicializado_ = true;
	return true;
}

void Transform::setParent(Transform* par)
{
	// Si teniamos otro padre, nos quitamos como hijos
	if (_parent != nullptr)
		_parent->removeChild(this);

	// Nos ponemos como hijos del nuevo padre
	if (par != nullptr)
		par->setChild(this);

	_parent = par;
	//_position = inverseTransformDirection(_parent->position());
	//_rotation = inverseTransformRotation(_parent->rotation());
	//_scale = inverseTransformDirection(_parent->scale());
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

