#include "PhysxManager.h"
#include "PsArray.h"
#include "Transform.h"

#include <windows.h> // LARGE_INTEGER ...
////#include <winnt.h>
////#include <profileapi.h>

#define PVD_HOST "127.0.0.1"	//Set this to the IP address of the system running the PhysX Visual Debugger that you want to connect to.
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

///#include "callbacks.hpp"

PhysxManager::PhysxManager() : _patata(false) 
{
	//StartCounter();

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

void PhysxManager::debugTime()
{
	std::cout << "PCFreq : " << PCFreq << std::endl;
	std::cout << "CounterStart : " << CounterStart << std::endl;
	std::cout << "CounterLast : " << CounterLast << std::endl;
	std::cout << "GetLastTime() = " << GetLastTime() << std::endl;
	std::cout << "GlobalTimer : " << GlobalTimer << std::endl;
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

	testBALL = createBall();
	StartCounter();
}

// interactive = game is rendering
// t = time passed since last call in ms
void PhysxManager::update(bool interactive, double t)
{
	if (mPause/* && !gOneFrame*/)
		return;
	//gOneFrame = false;

	stepPhysics(interactive, 1.0f/60.0f);

	// GetLastTime() 
	// Comienza con una media de 15.05ms si se resetea con CounterLast = CounterStart,
	// pero su uso real es llevar el tiempo real de la app activa.
	if (GetLastTime() - GlobalTimer > 1.0) {
		PxVec3 v = testBALL->getGlobalPose().p;
		std::cout << "PhyBALL position : ";
		std::cout << "( " << v.x << " , " << v.y << " , " << v.z << " )" << std::endl;
		debugTime();
		GlobalTimer = GetLastTime();
	}

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

	releaseScene();
	//PX_RELEASE(mScene);
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
void PhysxManager::stepPhysics(bool interactive, double frameTime)
{
	PX_UNUSED(interactive);

	mScene->simulate(frameTime/*1.0f / 60.0f*/);
	mScene->fetchResults(true);
}

void PhysxManager::onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}

std::ostream& operator<<(std::ostream& os, const physx::PxVec3& v) {
	os << "(" << v.x << "," << v.y << "," << v.z << ")";
	return os;
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

// ---------------- PREFABS --------------------------------------------------

PxRigidStatic* PhysxManager::createTriggerStaticBox(const PxVec3 halfExtent, const PxTransform& transform)
{
	PxShape* shape = createTriggerShape(PxBoxGeometry(halfExtent), *mMaterial, false);
	PxRigidStatic* body = createStaticRigid(transform, shape);
	shape->release(); // importante! (cuando ya se ha usado hay que borrarlo)
	return body;
}

PxRigidDynamic* PhysxManager::createBall()
{
	PxTransform transform = PxTransform(PxVec3(0, 400, 100));
	PxVec3 velocity = PxVec3(0, -50, -100);
	PxRigidDynamic* ball = createDynamic(transform, PxSphereGeometry(10), *mMaterial, velocity);
	PxRigidBodyExt::updateMassAndInertia(*ball, 1000.f); //density = 1000 ¡arrasa con todo!
	return ball;
}

void PhysxManager::createStackBoxes(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mMaterial);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			//Esta linea sobra, pero es un ejemplo de Dynamic con solo transform:
			//PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = createDynamic(t.transform(localTm), shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		}
	}
	shape->release();
}

void PhysxManager::tiledStacks(PxReal num, PxReal sideLength)
{
	for (PxU32 i = 0; i < num; i++)
		createStackBoxes(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 20 / sideLength, sideLength);
}

////----
float stepTime = 0.0f;
//#define FIXED_STEP
//
void PhysxManager::runPhysX()
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

void PhysxManager::actualizaTransform(Transform& tr, const PxRigidActor& body)
{
	PxTransform bodyTr = body.getGlobalPose();
	bodyTr.getNormalized();

	PxVec3 bodyPos = PxVec3(bodyTr.p);
	PxQuat bodyRot = PxQuat(bodyTr.q);

	Vectola3D pos = Vectola3D(bodyPos.x, bodyPos.y, bodyPos.z);
	tr.setPosition(pos);

	Quaterniola rot = Quaterniola(bodyRot.x, bodyRot.y, bodyRot.z, bodyRot.w);
	tr.setRotation(rot);
}

// Elimina de forma robusta todos los RigidBody's de la Scene.
void PhysxManager::releaseScene()
{
	PxActorTypeFlags actorTypes = PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC;
	shdfnd::Array<PxActor*> actorArray;

	PxU32 numActors = mScene->getNbActors(actorTypes);
	actorArray.resize(numActors);

	mScene->getActors(actorTypes, actorArray.begin(), actorArray.size());

	PxActor** actores = new PxActor*[numActors];
	for (int i = 0; i < numActors; i++) {
		actores[i] = actorArray[i];
	}

	mScene->removeActors(actores, numActors, false);
	PX_RELEASE(mScene);
}

// Elimina un RigidBody concreto de la Scene.
void PhysxManager::releaseBody(PxActor& body)
{
	mScene->removeActor(body, false);
}
////----