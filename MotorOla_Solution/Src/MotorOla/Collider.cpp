#include "Collider.h"
#include "PhysxManager.h"

Collider::Collider() : PxShape(concreteType_, baseFlags_)
{}

Collider::~Collider()
{
}

bool Collider::init(const std::map<std::string, std::string>& mapa)
{
	// comprobar que la sección existe
	if (mapa.find("type") == mapa.end())
		return false;

	// interpretar
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
		setGeometry(PxSphereGeometry(rad)); /// ¿escala?
	}
	else { ////PxGeometryType::eBOX////
		// comprobar que la sección existe
		if (mapa.find("x") == mapa.end() ||
			mapa.find("y") == mapa.end() ||
			mapa.find("z") == mapa.end())
			return false;

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
		setGeometry(PxBoxGeometry(PxVec3(dimX, dimY, dimZ))); /// ¿escala?
	}

	// Establece el tipo de simulacion fisica
	/// TODO LUA ?
	setFlag(PxShapeFlag::eSIMULATION_SHAPE, true); // for collision simulation
	//setFlag(PxShapeFlag::eTRIGGER_SHAPE); // for triggers

	// Establece el tipo de material
	PxMaterial* gMaterial = physX()->createMaterial(0.5f, 0.5f, 0.6f);
	setMaterials(&gMaterial, 1); // por ejemplo

	// En caso de no heredar y guardar el puntero: (mas breve)
	//PxGeometry* geo = new PxSphereGeometry(10);
	//physX()->createShape(*geo, *gMaterial);

	return false;
}