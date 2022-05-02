#include "Collider.h"
#include "PhysxManager.h"
#include "RigidBody.h"

Collider::Collider()
{
	//std::cout << "CONSTRUCTOR VACIO COLLIDER" << std::endl;
}

Collider::~Collider()
{
}

bool Collider::init(const std::map<std::string, std::string>& mapa)
{
	std::cout << "init COLLIDER, id:" << getEntidad()->getID() << std::endl;

	// comprobar que la sección existe
	if (mapa.find("type") == mapa.end())
		return false;

	// variables principales
	PxGeometry* geometry = nullptr;
	PxMaterial* material = pm().getMaterial();	// Establece el tipo de material

	// identifica el tipo de geometría
	std::string typeString = mapa.at("type");
	std::cout << "type geometry = " << typeString << std::endl;

	if (typeString == "sphere") {
		// comprobar que la sección existe
		if (mapa.find("radius") == mapa.end())
			return false;

		// establecemos config. de geometría
		type_ = PxGeometryType::eSPHERE;

		// traducción
		std::string radiusString = mapa.at("radius");
		float rad = stof(radiusString);
		//
		if (rad < 0)
			return false;

		// Crea la forma del collider (esfera)
		geometry = new PxSphereGeometry(rad); /// ¿la escala es igual respecto a Ogre?
	}
	else if (typeString == "box") {
		// comprobar que la sección existe
		if (mapa.find("x") == mapa.end() ||
			mapa.find("y") == mapa.end() ||
			mapa.find("z") == mapa.end())
				return false;

		// establecemos config. de geometría
		type_ = PxGeometryType::eBOX;

		// traducción
		std::string xString = mapa.at("x");
		float dimX = stof(xString);
		std::string yString = mapa.at("y");
		float dimY = stof(yString);
		std::string zString = mapa.at("z");
		float dimZ = stof(zString);
		//
		if (dimX < 0 || dimY < 0 || dimZ < 0)
			return false;

		// Crea la forma del collider (cubo)
		geometry = new PxBoxGeometry(PxVec3(dimX, dimY, dimZ)); /// ¿escala?
	}
	
	/*
	if (mapa.find("trigger") == mapa.end())
		return false; // salta esto si no lo encuentra...
	*/

	// Localiza el parametro que indica si el collider es tipo 'trigger'
	if (mapa.find("trigger") != mapa.end()) 
	{
		// Establece el tipo de simulacion fisica
		std::string triggerString = mapa.at("trigger");
		if (triggerString == "true")
			shape = pm().createTriggerShape(*geometry, *material, true);
	}
	// Si no se ha definido dicho parametro o es 'false'
	if (shape == nullptr) {
		shape = pm().createShape(*geometry, *material, true); // NO TRIGGER, NO POO
	}

	// Recoge si existe el componente RigidBody
	RigidBody* body = getEntidad()->getComponent<RigidBody>();
	if (body->getBody() && body->getBody()->getNbShapes() == 0)				// dynamic
		body->getBody()->attachShape(*shape);
	else if (body->getStBody() && body->getStBody()->getNbShapes() == 0)	// static
		body->getStBody()->attachShape(*shape);
	else
		std::cout << "Collider: no body, no shape attach" << std::endl;

	return true;
}