#include "RigidBody.h"
#include "PhysxManager.h"
#include "Collider.h"

RigidBody::RigidBody()
{
	std::cout << "CREANDO RIGIDBODY" << std::endl;
}

RigidBody::~RigidBody()
{
}

bool RigidBody::init(const std::map<std::string, std::string>& mapa)
{
	float positionX, positionY, positionZ;
	float orientationX, orientationY, orientationZ, orientationW;

	// comprobar que la sección existe
	if (mapa.find("posX") == mapa.end() ||
		mapa.find("posY") == mapa.end() ||
		mapa.find("posZ") == mapa.end() ||
		mapa.find("oriX") == mapa.end() ||
		mapa.find("oriY") == mapa.end() ||
		mapa.find("oriZ") == mapa.end() ||
		mapa.find("oriW") == mapa.end())
	{
		std::cout << "Rigidbody - posicion por defecto!" << std::endl;
	}
	else {
		// posición
		std::string pxString = mapa.at("posX");
		positionX = stof(pxString);
		std::string pyString = mapa.at("posY");
		positionY = stof(pyString);
		std::string pzString = mapa.at("posZ");
		positionZ = stof(pzString);
		// orientación
		std::string oxString = mapa.at("oriX");
		orientationX = stof(oxString);
		std::string oyString = mapa.at("oriY");
		orientationY = stof(oyString);
		std::string ozString = mapa.at("oriZ");
		orientationZ = stof(ozString);
		std::string owString = mapa.at("oriW");
		orientationW = stof(owString);

		// establece las propiedades
		position_ = PxVec3(positionX, positionY, positionZ);
		orientation_ = PxQuat(orientationX, orientationY, orientationZ, orientationW);
	}

	// Position and orientation in world space (Physx pose)
	PxTransform pose = PxTransform(position_, orientation_);

	// Establece si es estatico o dinamico
	bool estatico = false;
	if (mapa.find("static") == mapa.end())
		return false;
	else {
		// Establece el tipo de simulacion fisica
		std::string staticString = mapa.at("static");
		if (staticString == "true")
			estatico = true;
	}

	// Crea el RigidBody con todos los parametros recogidos
	PxShape* shape = getEntidad()->getComponent<Collider>()->getShape();
	if (shape) {
		if (estatico)
			stBody = pm().createStaticRigid(pose, shape);
		else
			body = pm().createDynamic(pose, shape);
		std::cout << "Rigidbody - collider!" << std::endl;
	}
	else {
		if (estatico)
			stBody = physX()->createRigidStatic(pose);
		else
			body = physX()->createRigidDynamic(pose);		
		std::cout << "Rigidbody - sin collider!" << std::endl;
	}

	return true;
}