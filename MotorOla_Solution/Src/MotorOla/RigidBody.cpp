#include "RigidBody.h"
#include "PhysxManager.h"
#include "Collider.h"
#include "Transform.h"

RigidBody::RigidBody()
{
	if (debugCom) std::cout << "CONSTRUCTOR VACIO RIGIDBODY" << std::endl;
}

RigidBody::~RigidBody()
{
}

bool RigidBody::init(const std::map<std::string, std::string>& mapa)
{
	/*if (no1stTry)
	{
		no1stTry = false;
		return false;
	}*/

	if (debugCom) std::cout << "init RIGIDBODY, id:" << getEntidad()->getID() << std::endl;

	// Recoge si existe el componente transform
	Transform* tr = getEntidad()->getComponent<Transform>();

	// Valores para guardar los datos leidos
	float posX=0.f, posY=0.f, posZ=0.f;
	float oriX=0.f, oriY=0.f, oriZ=0.f, oriW=1.f;
	float velX=0.f, velY=0.f, velZ=0.f;

	// Valores auxiliares para guardar el tama�o de cada string
	std::string::size_type sact = 0, stot = 0;
	std::string tempString; // string auxiliar para guardar la subcadena actual

	// Si no existe / no tiene el componente Transform, lee los datos de LUA
	if (!tr || tr->getLocalPosition() == Vectola3D()) {
		// comprobar que la secci�n existe
		if (mapa.find("position") != mapa.end())
		{
			// Lee los datos de la posici�n
			std::string posString = mapa.at("position");
			posX = stof(posString, &sact); stot = sact + 1; sact = 0;
			tempString = posString.substr(stot);
			posY = stof(tempString, &sact); stot += sact + 1; sact = 0;
			tempString = posString.substr(stot);
			posZ = stof(tempString, &sact); stot += sact + 1; sact = 0;

			// Establece las propiedades leidas...
			pos_ = PxVec3(posX, posY, posZ);
			if (debugCom) std::cout << "Position RB = " << pos_ << std::endl;
		}

		// comprobar que la secci�n existe
		if (mapa.find("orientation") != mapa.end())
		{
			// Lee los datos de la orientaci�n
			std::string oriString = mapa.at("orientation");
			oriX = stof(oriString, &sact); stot = sact + 1; sact = 0;
			tempString = oriString.substr(stot);
			oriY = stof(tempString, &sact); stot += sact + 1; sact = 0;
			tempString = oriString.substr(stot);
			oriZ = stof(tempString, &sact); stot += sact + 1; sact = 0;
			tempString = oriString.substr(stot);
			oriW = stof(tempString, &sact); stot += sact + 1; sact = 0;

			// Establece las propiedades leidas...
			ori_ = PxQuat(oriX, oriY, oriZ, oriW);
			if (debugCom) std::cout << "Orientation RB = " << ori_ << std::endl;
		}
	}
	else {
		if (debugCom) std::cout << "\nLectura directa de Transform  para RigidBody" << std::endl;
		Vectola3D pos = tr->getLocalPosition();
		Quaterniola rot = tr->getLocalRotation();
		//std::cout << "Position TR = " << pos << std::endl;
		//std::cout << "Orientation TR = " << rot << std::endl;
		//if (debugCom) std::cout << "Orientation TR = " << rot;// << std::endl;
		pos_ = PxVec3(pos.getX(), pos.getY(), pos.getZ());
		ori_ = PxQuat(rot.v.getX(), rot.v.getX(), rot.v.getX(), rot.s);
		if (debugCom) std::cout << "Position RB = " << pos_ << std::endl;
		if (debugCom) std::cout << "Orientation RB = " << ori_ << std::endl << std::endl;
	}

	// comprobar que la secci�n existe
	if (mapa.find("velocity") != mapa.end())
	{
		// Lee los datos de la velocidad
		std::string velString = mapa.at("velocity");
		velX = stof(velString, &sact); stot = sact + 1; sact = 0;
		tempString = velString.substr(stot);
		velY = stof(tempString, &sact); stot += sact + 1; sact = 0;
		tempString = velString.substr(stot);
		velZ = stof(tempString, &sact); stot += sact + 1; sact = 0;

		// Establece las propiedades leidas...
		vel_ = PxVec3(velX, velY, velZ);
		if (debugCom) std::cout << "Velocity RB = " << vel_ << std::endl;
	}	

	// Position and orientation in world space (Physx pose)
	PxTransform pose = PxTransform(pos_, ori_);

	// Establece si es estatico o dinamico
	bool estatico = false;
	if (mapa.find("static") != mapa.end())
	{
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
			body = pm().createDynamic(pose, shape, vel_);
		if (debugCom) std::cout << "Rigidbody - collider!" << std::endl;
	}
	else {
		if (estatico)
			stBody = pm().createStaticRigid(pose);
		else
			body = pm().createDynamic(pose, vel_);
		if (debugCom) std::cout << "Rigidbody - sin collider!" << std::endl;
	}

	// Establece la densidad del objeto
	float density = 1000.f;
	if (mapa.find("density") != mapa.end())
	{
		// Establece la masa y la inercia
		std::string densityString = mapa.at("density");
		density = stof(densityString);
		if (body) PxRigidBodyExt::updateMassAndInertia(*body, density);
		else {
			if (debugCom) std::cout << "RigidBody - ERROR: un static no usa density!" << std::endl;
			return false; // solo para dynamic!
		}
	}

	// A�ade la entidad asociado al manager de phyx
	int id_ = getEntidad()->getID();
	pm().addEntityID(id_);

	return true;
}

void RigidBody::setVelocity(PxVec3 v)
{
	vel_ = v;
	body->setLinearVelocity(v);
}

void RigidBody::setAngularVelocity(PxVec3 av)
{
	body->setAngularVelocity(av);
}
