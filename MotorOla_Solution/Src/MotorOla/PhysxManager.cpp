#include "PhysxManager.h"

// _patata = interactive

PhysxManager::PhysxManager() : _patata(false) {
	PX_UNUSED(_patata);

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	if (gFoundation == NULL) throw "PhysX-foundation no se ha iniciado correctamente.";
	else std::cout << std::endl << "PhysX-foundation INICIALIZADO!!" << std::endl;

	/* Si se quiere utilizar esta herramienta Pvd = PhysX Visual Debugger,
	* la cual es una herramienta externa, habría que resolver los RuntimeLibrary que genera esto:
	**
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);*/

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	if (gPhysics == NULL) throw "PhysX no se ha iniciado correctamente.";
	else std::cout << "PhysX INICIALIZADO!!\n" << std::endl;
}

PhysxManager::~PhysxManager()
{

}

void PhysxManager::update()
{

}

void PhysxManager::close()
{

}


