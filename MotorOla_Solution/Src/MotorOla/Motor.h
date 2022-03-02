#pragma once
class prueba;
class Motor
{
private:
	prueba* p;
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

