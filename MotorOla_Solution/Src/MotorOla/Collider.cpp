#include "Collider.h"
#include "PhysxManager.h"

Collider::Collider(PxType concreteType, PxBaseFlags baseFlags, PxGeometryType::Enum type) : PxShape(concreteType, baseFlags)
{
	// Crea la forma del collider (por defecto un cubo)
	if (type == PxGeometryType::eBOX)
		setGeometry(PxBoxGeometry(PxVec3(PxZero)));

	// Establece el tipo de simulacion fisica
	setFlag(PxShapeFlag::eSIMULATION_SHAPE, true); // for collision simulation
	//setFlag(PxShapeFlag::eTRIGGER_SHAPE); // for triggers

	// Establece el tipo de material
	PxMaterial* gMaterial = physX()->createMaterial(0.5f, 0.5f, 0.6f);
	setMaterials(&gMaterial, 1); // por ejemplo

	// En caso de no heredar y guardar el puntero: (mas breve)
	//PxGeometry* geo = new PxSphereGeometry(10);
	//physX()->createShape(*geo, *gMaterial);
}

Collider::~Collider()
{
}

bool Collider::init(const std::map<std::string, std::string>& mapa)
{
	// TODO: lectura del archivo .lua
	return false;
}