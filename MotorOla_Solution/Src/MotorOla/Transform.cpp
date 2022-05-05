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
	if (mapa.find("parent") == mapa.end()
		|| mapa.find("position") == mapa.end()
		|| mapa.find("rotation") == mapa.end()
		|| mapa.find("scale") == mapa.end())
			return false;
	std::string::size_type sz = 0, sa = 0;

	std::string parentString = mapa.at("parent");
	if (std::stoi(parentString) >= 0) {
		setParent(Singleton<EntidadManager>::instance()->getEntidadByID(stoi(parentString))->getComponent<Transform>());
	}

	std::string posString = mapa.at("position");
	float a = stof(posString, &sz);
	std::string temp = posString.substr(sz + 1);
	float b = stof(temp, &sa);
	float c = stof(posString.substr(sz + sa + 2));
	_localPosition = { a, b, c };

	std::string s2 = mapa.at("rotation");
	_localRotation = Quaterniola::Euler({ stof(s2, &sz), stof(s2.substr(sz + 1), &sa), stof(s2.substr(sz + sa + 2)) });

	std::string s3 = mapa.at("scale");
	_localScale = { stof(s3, &sz), stof(s3.substr(sz + 1), &sa), stof(s3.substr(sz + sa + 2)) };
	
	if (_parent == nullptr) {
		_position = _localPosition;
		_rotation = _localRotation;
		_scale = _localScale;
	}

	_inicializado = true;
	return true;
}

Transform* Transform::findChild(char* name)
{
	for (auto c : _children) {
		if (c->getEntidad()->getName() == name)
			return c;
	}
	return nullptr;
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
	_position = inverseTransformDirection(_parent->getPosition());
	_rotation = inverseTransformRotation(_parent->getRotation());
	_scale = inverseTransformDirection(_parent->getScale());
}

void Transform::setPosition(Vectola3D v) {
	_position = v;
	_localPosition = inverseTransformDirection(v);
}

void Transform::setPosition(float x, float y, float z) {
	_position = { x, y, z };
	_localPosition = inverseTransformDirection(x, y, z);
}

void Transform::setRotation(Quaterniola q) {
	_rotation = q;
	_localRotation = inverseTransformRotation(q);
}

Quaterniola Transform::inverseTransformRotation(Quaterniola q) {
	if (getParent())
		return q.conjugate() * getParent()->getRotation() * q;
	return q;
}

Quaterniola Transform::transformRotation(Quaterniola q) {
	if (getParent())
		return  getParent()->getRotation() * q;
	return q;
}

void Transform::setRotation(float x, float y, float z) {

	_rotation = Quaterniola::Euler({ x, y, z });
	_localRotation = inverseTransformRotation(_rotation);
}

void Transform::setScale(Vectola3D v) {
	_scale = v;
	_localScale = inverseTransformDirection(v);

	for (auto c : _children) {
		c->setScale(v + c->getLocalScale());
	}
}

void Transform::setScale(float x, float y, float z) {
	_scale = { x, y, z };
	_localScale = inverseTransformDirection(x, y, z);

	for (auto c : _children) {
		c->setScale(Vectola3D(x, y, z) + c->getLocalScale());
	}
}

void Transform::setLocalPosition(Vectola3D v) {
	_localPosition = v;
	_position = transformDirection(v);

	for (auto c : _children) {
		c->setPosition(_position + c->getLocalPosition());
	}
}

void Transform::setLocalPosition(float x, float y, float z) {
	_localPosition = { x, y, z };
	_position = transformDirection(x, y, z);

	for (auto c : _children) {
		c->setPosition(_position + c->getLocalPosition());
	}
}

void Transform::setLocalRotation(Quaterniola q) {
	_localRotation = q;
	_rotation = transformRotation(q);

	for (auto c : _children) {
		c->setRotation(_rotation.conjugate() * c->getLocalRotation());
	}
}

void Transform::setLocalRotation(float x, float y, float z) {
	_localRotation = Quaterniola::Euler({ x, y, z });
	_rotation = transformRotation(_localRotation);

	for (auto c : _children) {
		c->setRotation(_rotation.conjugate() * c->getLocalRotation());
	}
}

void Transform::setLocalScale(Vectola3D v) {
	_localScale = v;
	_scale = transformDirection(v);

	for (auto c : _children) {
		c->setScale(_scale + c->getLocalScale());
	}
}

void Transform::setLocalScale(float x, float y, float z) {
	_localScale = { x, y, z };
	_scale = transformDirection(x, y, z);

	for (auto c : _children) {
		c->setScale(_scale + c->getLocalScale());
	}
}

Vectola3D Transform::transformDirection(Vectola3D direction) {
	if (_parent == nullptr)
		return direction;

	else
		return { abs(direction.getX() + _parent->getLocalPosition().getX()),
				 abs(direction.getY() + _parent->getLocalPosition().getY()),
				 abs(direction.getZ() + _parent->getLocalPosition().getZ()) };
}

Vectola3D Transform::transformDirection(float x, float y, float z) {
	if (_parent == nullptr)
		return { x, y, z };

	else
		return { abs(x + _parent->getLocalPosition().getX()),
				 abs(y + _parent->getLocalPosition().getY()),
				 abs(z + _parent->getLocalPosition().getZ()) };
}

Vectola3D Transform::inverseTransformDirection(Vectola3D direction) {
	if (_parent == nullptr)
		return direction;

	else
		return { abs(direction.getX() - _parent->getLocalPosition().getX()),
				 abs(direction.getY() - _parent->getLocalPosition().getY()),
				 abs(direction.getZ() - _parent->getLocalPosition().getZ()) };
}

Vectola3D Transform::inverseTransformDirection(float x, float y, float z) {
	if (_parent == nullptr)
		return { x, y, z };

	else
		return { abs(x - _parent->getLocalPosition().getX()),
				 abs(y - _parent->getLocalPosition().getY()),
				 abs(z - _parent->getLocalPosition().getZ()) };
}

void Transform::getParentData()
{
	if (getParent()) {
		_rotation = getParent()->getRotation() * _localRotation;

		Vectola3D scale;
		scale.setX(getParent()->getLocalScale().getX() *_localScale.getX());
		scale.setY(getParent()->getLocalScale().getY() * _localScale.getY());
		scale.setZ(getParent()->getLocalScale().getZ() * _localScale.getZ());

		_scale = scale;
		_position = getParent()->getPosition() + _localPosition;
	}
	else {
		_rotation = _localRotation;
		_scale = _localScale;
		_position = _localPosition;
	}
}

void Transform::translate(double x, double y, double z)
{
	_position.setX(_position.getX() + x);
	_position.setY(_position.getY() + y);
	_position.setZ(_position.getZ() + z);
}

void Transform::rotate(float xAngle, float yAngle, float zAngle, Space relativeTo)
{
	Quaterniola rot(Quaterniola::Euler({ xAngle, yAngle, zAngle }));
	switch (relativeTo) {
	case Space::Self:
		_rotation = _rotation * rot;
		break;
	case Space::Parent:
		_rotation = rot * _rotation;
		break;
	case Space::World:
		getParentData();
		_rotation = _rotation * _localRotation.inverse() * rot * _localRotation;
		break;
	default:
		break;
	}

	_rotation.normalize();
}
