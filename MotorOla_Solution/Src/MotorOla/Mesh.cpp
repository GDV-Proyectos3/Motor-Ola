#include "Mesh.h"
#include "Entidad.h"
#include "Transform.h"
#include <OgreBillboardSet.h>

Mesh::Mesh(Ogre::SceneNode* node) : _nodo(node)
{
	_sceneManager = _nodo->getCreator();
}

Mesh::Mesh(Ogre::SceneNode* node,std::string mesh, std::string material) : _nodo(node)
{
	_sceneManager = _nodo->getCreator();

	_entity = _sceneManager->createEntity(mesh);

	std::cout << "Entra a mesh\n";
	// RECORDAR DESCOMENTAR
	//ent->setMaterialName(material);

	_nodo->attachObject(_entity);
}

Mesh::~Mesh()
{
}

bool Mesh::getActive()
{
	return _active;
}

void Mesh::setActive(bool a)
{
	_active = a;
}

Ogre::SceneNode* Mesh::getNodo()
{
	return _nodo;
}

void Mesh::setMesh(std::string mesh)
{
	_entity = _sceneManager->createEntity(mesh);
	_nodo->attachObject(_entity);
}

void Mesh::setMaterial(std::string material)
{
	_entity->setMaterialName(material);
}

//void Mesh::setBillboard(std::string material, Vector2D pos, Vector2D scale)
//{
//	bbNode = _nodo->createChildSceneNode();
//	bbNode->setPosition(pos.getX(),0,pos.getY());
//	Ogre::BillboardSet* bbSet = _sceneManager->createBillboardSet(1);
//	bbSet->setDefaultDimensions(scale.getX(),scale.getY());
//	bbSet->setMaterialName(material);
//	bbNode->attachObject(bbSet);
//	Ogre::Billboard* bb = bbSet->createBillboard(Ogre::Vector3(pos.getX(), 0, pos.getY()));
//}
//
//void Mesh::toggleBillboard()
//{
//	bbNode->setVisible(true);
//}
//
//void Mesh::setParticle(std::string material)
//{
//	particleNode = _nodo->createChildSceneNode();
//	smokePS = _sceneManager->createParticleSystem(material);
//	smokePS->setEmitting(false);
//	particleNode->attachObject(smokePS);
//}
//
//void Mesh::toggleParticle()
//{
//	smokePS->setEmitting(true);
//}
//
//void Mesh::setLight()
//{
//	lightNode = _nodo->createChildSceneNode();
//
//	luzFoco = _sceneManager->createLight();
//	luzFoco->setType(Ogre::Light::LT_SPOTLIGHT);
//	luzFoco->setDiffuseColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
//	luzFoco->setSpotlightInnerAngle(Ogre::Degree(2.0f));
//	luzFoco->setSpotlightOuterAngle(Ogre::Degree(1.0f));
//	luzFoco->setSpotlightFalloff(0.0f);
//	luzFoco->setVisible(false);
//	lightNode->attachObject(luzFoco);
//	lightNode->setDirection(Ogre::Vector3(0, -1, 0));  //vec3.normalise();
//}
//
//void Mesh::toggleLight()
//{
//	luzFoco->setVisible(true);
//}

void Mesh::init() //en funcion de si el eje sobre el que esta el suelo es x,y o xz comentar/descomentar algunas de estas lineas
{
	if (entity_->hasComponent<Transform>())
	{
		Transform* tr = entity_->getComponent<Transform>();
		//mNode->setPosition(tr->getPos().getX(), tr->getPos().getY(), 0);
		_nodo->setPosition(tr->getPos().getX(), 0,tr->getPos().getY());
		_nodo->setScale(tr->getW(), tr->getH(), tr->getW());
		//mNode->setScale(tr->getW(), tr->getW(), tr->getH());
		_nodo->yaw(Ogre::Degree(tr->getRot()));
	}
}

void Mesh::update()
{

	if (entity_->hasComponent<Transform>())
	{
		Transform* tr = entity_->getComponent<Transform>();
		//mNode->setPosition(tr->getPos().getX(), tr->getPos().getY(), 0);
		_nodo->setPosition(tr->getPos().getX(), 0, tr->getPos().getY());
		_nodo->setScale(tr->getW(), tr->getH(), tr->getW());
		//mNode->setScale(tr->getW(), tr->getW(), tr->getH());
		_nodo->yaw(Ogre::Degree(tr->getRot()));
	}
}
