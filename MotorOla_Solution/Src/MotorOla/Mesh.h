#pragma once
#include "Componente.h"
#include "./utils/Vector2D.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include "utils/Vector2D.h"
#include <OgreParticleSystem.h>

class Mesh : public Componente
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
	void setBillboard(std::string material , Vector2D pos, Vector2D scale);
	void toggleBillboard();
	void setParticle(std::string material);
	void toggleParticle();
	void setLight();
	void toggleLight();

	void init();
	void update();
	//void draw();



private:
	Ogre::SceneNode* mNode;
	Ogre::SceneNode* bbNode;
	Ogre::SceneNode* particleNode;
	Ogre::ParticleSystem* smokePS;
	Ogre::SceneNode* lightNode;
	Ogre::Light* luzFoco;
	Ogre::SceneManager* mSM;
	Ogre::Entity* ent;
	bool active = true;

};

