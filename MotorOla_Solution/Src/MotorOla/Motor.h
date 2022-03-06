#pragma once

// Declaracion de clases necesarias
class OgreManager;

class Motor
{
private:
	
	//Ogre
	OgreManager* _ogreManager = nullptr;

	//Physx
	//FMod
	//Cegui
	bool stop = false;

public:
	Motor();
	~Motor();
	void initSystems();
	void updateSystems();
};

