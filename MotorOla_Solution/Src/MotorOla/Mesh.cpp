#include "Mesh.h"

Mesh::Mesh(Ogre::SceneNode* node) : mNode(node)
{
	mSM = mNode->getCreator();
}

Mesh::Mesh(Ogre::SceneNode* node,std::string mesh, std::string material) : mNode(node)
{
	mSM = mNode->getCreator();
	ent = mSM->createEntity(mesh);
	ent->setMaterialName(material);
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

void Mesh::init()
{
}

void Mesh::update()
{
}
