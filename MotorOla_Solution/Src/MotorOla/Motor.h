#pragma once

class Motor
{
private:
	
	//Ogre
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

