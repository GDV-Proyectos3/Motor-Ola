#include "PhysxManager.h"

#include <windows.h> // LARGE_INTEGER ...
////#include <winnt.h>
////#include <profileapi.h>

#define PVD_HOST "127.0.0.1"	//Set this to the IP address of the system running the PhysX Visual Debugger that you want to connect to.
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

///#include "callbacks.hpp"

PhysxManager::PhysxManager() : _patata(false) 
{
	StartCounter();

	PX_UNUSED(true);
	//PX_UNUSED(_patata);

	// General settings
	scale.length = 100;        // typical length of an object
	scale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
}

PhysxManager::~PhysxManager()
{
	/*PX_RELEASE(mPhysics);
	PX_RELEASE(mFoundation);*/
}

// -------------- TIMER ----------------------------------------------------------------

void PhysxManager::StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return;

	PCFreq = double(li.QuadPart) /*/ 1000.0*/;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	CounterLast = CounterStart;
}

double PhysxManager::GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double t = double(li.QuadPart - CounterLast) / PCFreq;
	CounterLast = li.QuadPart;
	return t;
}

double PhysxManager::GetLastTime()
{
	double t = double(CounterLast - CounterStart) / PCFreq;
	return t;
}

// ------------ MAIN SINGLETON -------------------------------------------------------

void PhysxManager::init()
{
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

// interactive = game is rendering
// t = time passed since last call in ms
void PhysxManager::update(bool interactive, double t)
{
	stepPhysics(interactive, t);

	//spawn objects
	///....
	//update objects
	///....
	//delete objects that reached max. TTL seconds
	///....
}

// Function to clean data
// Add custom code to the begining of the function
// interactive = game is rendering
void PhysxManager::close(bool interactive)
{
	// delete
	///....

	PX_UNUSED(interactive);

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

// ---------------- UTILS --------------------------------------------------

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void PhysxManager::stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	mScene->simulate(t);
	mScene->fetchResults(true);
}

void PhysxManager::onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}

// ---------------- FACTORY --------------------------------------------------

void PhysxManager::createBall(
	const PxTransform& t = PxTransform(PxVec3(0, 40, 100)), 
	const PxGeometry& geometry = PxSphereGeometry(10), 
	const PxVec3& velocity = PxVec3(0, -50, -100))
{
	PxMaterial* gMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, t, geometry, *gMaterial, 10.0f);
	//dynamic->setAngularDamping(0.5f);
	//dynamic->setLinearVelocity(velocity);
	//mScene->addActor(*dynamic);
}

void PhysxManager::attachBola(Entidad* ball)
{
}


////----
float stepTime = 0.0f;
//#define FIXED_STEP
//
void PhysxManager::renderCallback()
{
	double t = GetCounter();
#ifdef FIXED_STEP
	if (t < (1.0f / 30.0f))
	{
		fprintf(stderr, "Time: %f\n", stepTime);
		stepTime += t;
	}
	else
		stepTime = 1.0f / 30.0f;

	if (stepTime >= (1.0f / 30.0f))
	{
		PhysxManager::update(true, stepTime);
		stepTime = 0.0f;
	}
#else
	PhysxManager::update(true, t);
#endif
}
////----