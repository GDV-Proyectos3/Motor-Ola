#include "Mesh.h"
#include "Entidad.h"
#include "Transform.h"
#include <OgreBillboardSet.h>

Mesh::Mesh(Ogre::SceneNode* node) : mNode(node)
{
	mSM = mNode->getCreator();
}

Mesh::Mesh(Ogre::SceneNode* node,std::string mesh, std::string material) : mNode(node)
{
	mSM = mNode->getCreator();
	ent = mSM->createEntity(mesh);

	// RECORDAR DESCOMENTAR
	//ent->setMaterialName(material);

	mNode->attachObject(ent);
}

Mesh::~Mesh()
{
}

bool Mesh::getActive()
{
	return active;
}

void Mesh::setActive(bool a)
{
	active = a;
}

Ogre::SceneNode* Mesh::getNodo()
{
	return mNode;
}

void Mesh::setMesh(std::string mesh)
{
	ent = mSM->createEntity(mesh);
	mNode->attachObject(ent);
}

void Mesh::setMaterial(std::string material)
{
	ent->setMaterialName(material);
}

void Mesh::setBillboard(std::string material, Vector2D pos, Vector2D scale)
{
	bbNode = mNode->createChildSceneNode();
	bbNode->setPosition(pos.getX(),0,pos.getY());
	Ogre::BillboardSet* bbSet = mSM->createBillboardSet(1);
	bbSet->setDefaultDimensions(scale.getX(),scale.getY());
	bbSet->setMaterialName(material);
	bbNode->attachObject(bbSet);
	Ogre::Billboard* bb = bbSet->createBillboard(Ogre::Vector3(pos.getX(), 0, pos.getY()));
}

void Mesh::toggleBillboard()
{
	bbNode->setVisible(true);
}

void Mesh::setParticle(std::string material)
{
	particleNode = mNode->createChildSceneNode();
	smokePS = mSM->createParticleSystem(material);
	smokePS->setEmitting(false);
	particleNode->attachObject(smokePS);
}

void Mesh::toggleParticle()
{
	smokePS->setEmitting(true);
}

void Mesh::setLight()
{
	lightNode = mNode->createChildSceneNode();

	luzFoco = mSM->createLight();
	luzFoco->setType(Ogre::Light::LT_SPOTLIGHT);
	luzFoco->setDiffuseColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	luzFoco->setSpotlightInnerAngle(Ogre::Degree(2.0f));
	luzFoco->setSpotlightOuterAngle(Ogre::Degree(1.0f));
	luzFoco->setSpotlightFalloff(0.0f);
	luzFoco->setVisible(false);
	lightNode->attachObject(luzFoco);
	lightNode->setDirection(Ogre::Vector3(0, -1, 0));  //vec3.normalise();
}

void Mesh::toggleLight()
{
	luzFoco->setVisible(true);
}



void Mesh::init() //en funcion de si el eje sobre el que esta el suelo es x,y o xz comentar/descomentar algunas de estas lineas
{
	if (entity_->hasComponent<Transform>())
	{
		Transform* tr = entity_->getComponent<Transform>();
		//mNode->setPosition(tr->getPos().getX(), tr->getPos().getY(), 0);
		mNode->setPosition(tr->getPos().getX(), 0,tr->getPos().getY());
		mNode->setScale(tr->getW(), tr->getH(), tr->getW());
		//mNode->setScale(tr->getW(), tr->getW(), tr->getH());
		mNode->yaw(Ogre::Degree(tr->getRot()));
	}
}

void Mesh::update()
{

	if (entity_->hasComponent<Transform>())
	{
		Transform* tr = entity_->getComponent<Transform>();
		//mNode->setPosition(tr->getPos().getX(), tr->getPos().getY(), 0);
		mNode->setPosition(tr->getPos().getX(), 0, tr->getPos().getY());
		mNode->setScale(tr->getW(), tr->getH(), tr->getW());
		//mNode->setScale(tr->getW(), tr->getW(), tr->getH());
		mNode->yaw(Ogre::Degree(tr->getRot()));
	}
}
