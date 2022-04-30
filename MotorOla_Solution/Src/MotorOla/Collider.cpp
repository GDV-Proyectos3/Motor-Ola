#include "Collider.h"
#include "PhysxManager.h"

Collider::Collider()
{
	std::cout << "CREANDO COLLIDER" << std::endl;
}

Collider::~Collider()
{
}

bool Collider::init(const std::map<std::string, std::string>& mapa)
{
	// comprobar que la sección existe
	if (mapa.find("type") == mapa.end())
		return false;

	// variables principales
	PxGeometry geometry = PxBoxGeometry();
	PxMaterial* material = pm().getMaterial();	// Establece el tipo de material

	// identifica el tipo de geometría
	std::string typeString = mapa.at("type");

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
		geometry = PxSphereGeometry(rad); /// ¿la escala es igual respecto a Ogre?
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
		geometry = PxBoxGeometry(PxVec3(dimX, dimY, dimZ)); /// ¿escala?
	}
	
	if (mapa.find("trigger") == mapa.end())
		return false;
	else {
		// Establece el tipo de simulacion fisica
		std::string triggerString = mapa.at("trigger");
		if (triggerString == "true")
			shape = pm().createTriggerShape(geometry, *material, true);
		else {
			shape = pm().createShape(geometry, *material, true);
		}
	}
	
	return true;
}