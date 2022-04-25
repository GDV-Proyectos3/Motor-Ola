#include "PhysxManager.h"
#define PVD_HOST "127.0.0.1"	//Set this to the IP address of the system running the PhysX Visual Debugger that you want to connect to.
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

///#include "callbacks.hpp"

PhysxManager::PhysxManager() : _patata(false) 
{
	PX_UNUSED(_patata);

	// General settings
	PxTolerancesScale scale;
	scale.length = 100;        // typical length of an object
	scale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice

	// Foundation --------------------------------------------------------------------
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAllocator, mErrorCallback);
	if (!mFoundation) 
		assert("PxCreateFoundation failed!");
	else std::cout << std::endl << "PxFoundation INICIALIZADO!!" << std::endl;

	// PhysX Visual Debugger ---------------------------------------------------------
	bool recordMemoryAllocations = true;

	mPvd = PxCreatePvd(*mFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5050, 10);
	mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	// Physics -----------------------------------------------------------------------
	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
		scale, recordMemoryAllocations, mPvd);
	if (!mPhysics)
		assert("PxCreatePhysics failed!");
	else std::cout << "PxPhysX INICIALIZADO!!\n" << std::endl;

	// Cooking -----------------------------------------------------------------------
	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(scale));
	if (!mCooking)
		assert("PxCreateCooking failed!");
	else std::cout << "PxCooking INICIALIZADO!!\n" << std::endl;

	// Scene for GPU Rigid Bodies ----------------------------------------------------
	mCuda = PxCreateCudaContextManager(*mFoundation, cudaDesc, PxGetProfilerCallback());

	PxSceneDesc sceneDesc(scale);
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	mDispatcher = PxDefaultCpuDispatcherCreate(4);
	sceneDesc.cpuDispatcher = mDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.cudaContextManager = mCuda;

	sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;

	mScene = mPhysics->createScene(sceneDesc);
}

PhysxManager::~PhysxManager()
{
	PX_RELEASE(mPhysics);
	PX_RELEASE(mFoundation);
}

void PhysxManager::update()
{
	////gScene->simulate(/*t*/0);
	////gScene->fetchResults(true);

	//spawn
	///....
	//update
	///....
	//refresh
	///....
}

void PhysxManager::close()
{
	// delete
	///....

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	mScene->release();
	mDispatcher->release();
	// -----------------------------------------------------
	mPhysics->release();
	PxPvdTransport* transport = mPvd->getTransport();
	mPvd->release();
	transport->release();

	mFoundation->release();
}

void PhysxManager::createBall(
	const PxTransform& t = PxTransform(PxVec3(0, 40, 100)), 
	const PxGeometry& geometry = PxSphereGeometry(10), 
	const PxVec3& velocity = PxVec3(0, -50, -100))
{
	PxMaterial* gMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, t, geometry, *gMaterial, 10.0f);
	//dynamic->setAngularDamping(0.5f);
	//dynamic->setLinearVelocity(velocity);
	//gScene->addActor(*dynamic);

}

void PhysxManager::attachBola(Entidad* ball)
{
}