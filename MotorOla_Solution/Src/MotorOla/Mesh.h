#pragma once
#include "Componente.h"
#include "./utils/Vector2D.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
class Mesh :   public Componente
{
public:
	Mesh(Ogre::SceneNode* node);
	Mesh(Ogre::SceneNode* node, std::string mesh, std::string material);
	virtual ~Mesh();

	bool  getActive();
	void  setActive(bool a) ;
	Ogre::SceneNode* getNodo() ;

	void setMesh(std::string mesh);
	void setMaterial(std::string material);

	void init();
	void update();
	//void draw();



private:
	Ogre::SceneNode* mNode;
	Ogre::SceneNode* bbNode;
	Ogre::SceneNode* particleNode;
	Ogre::SceneNode* lightNode;
	Ogre::SceneManager* mSM;
	Ogre::Entity* ent;
	bool active = true;

};

