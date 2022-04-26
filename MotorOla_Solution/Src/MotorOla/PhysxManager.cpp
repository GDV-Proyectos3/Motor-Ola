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
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
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

	// Cuda Context Manager ----------------------------------------------------------
	mCuda = PxCreateCudaContextManager(*mFoundation, cudaDesc, PxGetProfilerCallback());
	if (mCuda)
	{
		if (!mCuda->contextIsValid())
		{
			mCuda->release();
			mCuda = NULL;
		}
	}
#ifdef RENDER_SNIPPET
	cudaDesc.interopMode = PxCudaInteropMode::OGL_INTEROP;	//Choose interop mode. As the snippets use OGL, we select OGL_INTEROP
															//when using D3D, cudaContextManagerDesc.graphicsDevice must be set as the graphics device pointer.
#else
	cudaDesc.interopMode = PxCudaInteropMode::NO_INTEROP;
#endif

	// Scene for GPU Rigid Bodies ----------------------------------------------------
	PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	mDispatcher = PxDefaultCpuDispatcherCreate(4);				//Create a CPU dispatcher using 4 worther threads
	sceneDesc.cpuDispatcher = mDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	sceneDesc.cudaContextManager = mCuda;						//Set the CUDA context manager, used by GRB.

	sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;		//Enable GPU dynamics - without this enabled, simulation (contact gen and solver) will run on the CPU.
	sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;				//Enable PCM. PCM NP is supported on GPU. Legacy contact gen will fall back to CPU
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;		//Improve solver stability by enabling post-stabilization.
	sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;			//Enable GPU broad phase. Without this set, broad phase will run on the CPU.
	sceneDesc.gpuMaxNumPartitions = 8;							//Defines the maximum number of partitions used by the solver. Only power-of-2 values are valid. 
																//A value of 8 generally gives best balance between performance and stability.

	mScene = mPhysics->createScene(sceneDesc);

	// PVD client conection ----------------------------------------------------------
	PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, false);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, false);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, false);
	}

	// Inicialización de variables dependientes --------------------------------------
	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, PxPlane(0, 1, 0, 0), *mMaterial);
	mScene->addActor(*groundPlane);
}

// interactive = game is rendering
// t = time passed since last call in ms
void PhysxManager::update(bool interactive, double t)
{
	if (mPause/* && !gOneFrame*/)
		return;
	//gOneFrame = false;

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
	PX_UNUSED(interactive);

	PX_RELEASE(mScene);
	PX_RELEASE(mDispatcher);
	PX_RELEASE(mPhysics);

	if (mPvd)
	{
		PxPvdTransport* transport = mPvd->getTransport();
		mPvd->release();	mPvd = NULL;
		PX_RELEASE(transport);
	}

	PX_RELEASE(mCuda);
	PX_RELEASE(mFoundation);
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

PxRigidDynamic* PhysxManager::createDynamic(const PxTransform& transform, const PxGeometry& geometry, PxMaterial& material, const PxVec3& velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, transform, geometry, material, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	mScene->addActor(*dynamic);
	return dynamic;
}

PxRigidDynamic* PhysxManager::createDynamic(const PxTransform& transform, PxShape* shape, const PxVec3& velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, transform, *shape, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	mScene->addActor(*dynamic);
	return dynamic;
}

PxRigidStatic* PhysxManager::createStaticRigid(const PxTransform& transform, const PxGeometry& geom, PxMaterial& material)
{
	PxShape* shape = createTriggerShape(geom, material, false);
	PxRigidStatic* body = createStaticRigid(transform, shape);
	shape->release(); // importante! (cuando ya se ha usado hay que borrarlo)
	return nullptr;
}

PxRigidStatic* PhysxManager::createStaticRigid(const PxTransform& transform, PxShape* shape)
{
	PxRigidStatic* body = nullptr;

	if (shape)
	{
		body = mPhysics->createRigidStatic(transform);
		body->attachShape(*shape);
		mScene->addActor(*body);
	}

	return body;
}

PxShape* PhysxManager::createShape(const PxGeometry& geom, PxMaterial& material, bool isExclusive)
{
	PxShape* shape = nullptr;
	const PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eSIMULATION_SHAPE;
	shape = mPhysics->createShape(geom, material, isExclusive, shapeFlags);
	return shape;
}

PxShape* PhysxManager::createTriggerShape(const PxGeometry& geom, PxMaterial& material, bool isExclusive)
{
	PxShape* shape = nullptr;
	const PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eTRIGGER_SHAPE;
	shape = mPhysics->createShape(geom, material, isExclusive, shapeFlags);
	return shape;
}

PxRigidStatic* PhysxManager::createTriggerStaticBox(const PxVec3 halfExtent, const PxTransform& transform)
{
	PxShape* shape = createTriggerShape(PxBoxGeometry(halfExtent), *mMaterial, false);
	PxRigidStatic* body = createStaticRigid(transform, shape);
	shape->release(); // importante! (cuando ya se ha usado hay que borrarlo)
	return body;
}

PxRigidDynamic* PhysxManager::createBall()
{
	PxTransform transform = PxTransform(PxVec3(0, 40, 100));
	PxVec3 velocity = PxVec3(0, -50, -100);
	PxRigidDynamic* ball = createDynamic(transform, PxSphereGeometry(10), *mMaterial, velocity);
	PxRigidBodyExt::updateMassAndInertia(*ball, 1000.f); //density = 1000 ˇarrasa con todo!
	return ball;
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