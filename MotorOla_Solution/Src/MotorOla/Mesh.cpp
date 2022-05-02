#include "Mesh.h"
#include "Entidad.h"
#include "Transform.h"
#include "utils/Singleton.h"
#include "OgreManager.h"
#include <OgreBillboardSet.h>

Mesh::Mesh()
{
	if (debugCom) std::cout << "CONSTRUCTOR VACIO MESH" << std::endl;

	_sceneManager = Singleton<OgreManager>::instance()->getSceneManager();
	_nodo = _sceneManager->getRootSceneNode()->createChildSceneNode();
	//_sceneManager = _nodo->getCreator();
}

//Mesh::Mesh(Ogre::SceneNode* node) : _nodo(node)
//{
//	_sceneManager = _nodo->getCreator();
//}
//
//Mesh::Mesh(Ogre::SceneNode* node,std::string mesh, std::string material) : _nodo(node)
//{
//	_sceneManager = _nodo->getCreator();
//
//	// Crea la entity de Ogre
//	_OgreEntity = _sceneManager->createEntity(mesh);
//
//	// Asigna el material
//	_OgreEntity->setMaterialName(material);
//
//	// Junta la entity al nodo
//	_nodo->attachObject(_OgreEntity);
//}

Mesh::~Mesh()
{
	Singleton<OgreManager>::instance()->getSceneManager()->destroyEntity(_ogreEntity);
	Singleton<OgreManager>::instance()->getSceneManager()->destroySceneNode(_nodo);
}



bool Mesh::init(const std::map<std::string, std::string>& mapa)
{
	if (debugCom) std::cout << "init MESH, id:" << getEntidad()->getID() << std::endl;
	
	if (mapa.find("mesh") == mapa.end() || mapa.find("material") == mapa.end() || mapa.find("visible") == mapa.end()) return false;

	//_nodo = Singleton<OgreManager>::instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode();

	Transform* tr = entity_->getComponent<Transform>();
	if (tr == nullptr)
		return false;

	std::string me = mapa.at("mesh");
	setMesh(me);

	std::string ma = mapa.at("material");
	if (ma != "") setMaterial(ma);

	std::string vi = mapa.at("visible");
	if (vi == "true") setVisible(true);
	else if (vi == "false") setVisible(false);
	else return false;

	_nodo->setPosition(tr->getPosition().getX(), tr->getPosition().getY(), tr->getPosition().getZ());
	_nodo->setScale(tr->getScale().getX(), tr->getScale().getY(), tr->getScale().getZ());
	_nodo->setOrientation(tr->getRotation());

	inicializado_ = true;

	return true;
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
	_ogreEntity = _sceneManager->createEntity(mesh);
	_nodo->attachObject(_ogreEntity);
}

void Mesh::setMaterial(std::string material)
{
	_ogreEntity->setMaterialName(material);
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

void Mesh::update()
{

	if (entity_->hasComponent<Transform>())
	{
		Transform* tr = entity_->getComponent<Transform>();
		_nodo->setPosition(tr->getPosition().getX(), tr->getPosition().getY(), tr->getPosition().getZ());
		_nodo->setOrientation(tr->getRotation());
		_nodo->setScale(tr->getScale().getX(), tr->getScale().getY(), tr->getScale().getZ());
	}
}

void Mesh::setVisible(bool state)
{
	_visible = state;
	_nodo->setVisible(state);
}

