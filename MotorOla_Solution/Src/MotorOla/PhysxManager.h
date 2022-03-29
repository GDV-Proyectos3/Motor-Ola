#pragma once

#include "utils\Singleton.h"

//#include <string>

//namespace physx {
	// ...
//}

class PhysxManager : public Singleton<PhysxManager> {
	friend Singleton<PhysxManager>;
public:
	PhysxManager(/*...*/);
	~PhysxManager();

	// Getters
	// ...

	void init();
	void update();
	void close();

	// Etc
	// virtual void patata(...) {}

	// Funciones
	// void loadPatatas();

private:
	// Variables
	bool _patata;
};