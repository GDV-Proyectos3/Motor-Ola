#include "PhysxManager.h"
#include "PsArray.h"

#include <windows.h> // LARGE_INTEGER ...

#define PVD_HOST "127.0.0.1"	//Set this to the IP address of the system running the PhysX Visual Debugger that you want to connect to.
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

///#include "callbacks.hpp"
#include "EntidadManager.h"
#include "RigidBody.h"
#include "Transform.h"


std::unique_ptr<PhysxManager> Singleton<PhysxManager>::instance_ = nullptr;

PhysxManager::PhysxManager() : _patata(false)
{
	

	PX_UNUSED(true);
	

	// General settings
	scale.length = 100;        // typical length of an object
	scale.speed = 9.81;         // typical speed of an object, gravity*1s is a reasonable choice
}

PhysxManager::~PhysxManager()
{
	std::cout << "\n - Cerrando PhysXmanager - \n";
	close(false);
	std::cout << " - - - - - - - - - - - - - \n";
}

// -------------- TIMER ----------------------------------------------------------------

void PhysxManager::StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return;

	PCFreq = double(li.QuadPart);

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
	else std::cout << "\nPxFoundation INICIALIZADO!!\n";

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
	else std::cout << "PxPhysX INICIALIZADO!!\n\n";

	// Cooking -----------------------------------------------------------------------
	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(scale));
	if (!mCooking)
		assert("PxCreateCooking failed!");
	else std::cout << "PxCooking INICIALIZADO!!\n\n";

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
	
	sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);

	mDispatcher = PxDefaultCpuDispatcherCreate(4);					//Create a CPU dispatcher using 4 worther threads
	sceneDesc.cpuDispatcher = mDispatcher;

	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &mContactReportCallback;	//Create a system callback to manage collisions
			

	sceneDesc.cudaContextManager = mCuda;							//Set the CUDA context manager, used by GRB.

	sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;			//Enable GPU dynamics - without this enabled, simulation (contact gen and solver) will run on the CPU.
	sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;					//Enable PCM. PCM NP is supported on GPU. Legacy contact gen will fall back to CPU
	sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;			//Improve solver stability by enabling post-stabilization.
	sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;				//Enable GPU broad phase. Without this set, broad phase will run on the CPU.
	sceneDesc.gpuMaxNumPartitions = 8;								//Defines the maximum number of partitions used by the solver. Only power-of-2 values are valid. 
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

	// Inicializaci�n de variables dependientes --------------------------------------
	mMaterial = mPhysics->createMaterial(0.0f, 0.0f, 0.0f);
	StartCounter();
}

// interactive = game is rendering
// t = time passed since last call in ms
void PhysxManager::update(bool interactive, double t)
{
	if (mPause)
		return;

	// Actualiza las posiciones: Transform global --> PxTransform
	auto it = em().getAllEntidades().begin();
	while (it != em().getAllEntidades().end()) {
		Entidad* e = (*it).get();
		it++;
		if (e->getComponent<RigidBody>() == nullptr) continue;
		RigidBody* body = e->getComponent<RigidBody>();
		if (body->getBody()) setGlobalToPhysxTR(*e, *body->getBody());
	}

	// Actualiza las fisicas de movimiento y colisiones
	stepPhysics(interactive, 1.0f/60.0f);

	// Actualiza las posiciones: PxTransform --> Transform global
	it = em().getAllEntidades().begin();
	while (it != em().getAllEntidades().end()) {
		Entidad* e = (*it).get();
		it++;
		if (e->getComponent<RigidBody>() == nullptr) continue;
		RigidBody* body = e->getComponent<RigidBody>();
		if (body->getBody()) setPhysxToGlobalTR(*e, *body->getBody());
	}
}

// Function to clean data
// Add custom code to the begining of the function
// interactive = game is rendering
void PhysxManager::close(bool interactive)
{
	PX_UNUSED(interactive);

	if (mScene != NULL) PX_RELEASE(mScene);
	PX_RELEASE(mDispatcher);
	PxCloseExtensions();
	PX_RELEASE(mPhysics);

	if (mPvd)
	{
		PxPvdTransport* transport = mPvd->getTransport();
		PX_RELEASE(mPvd);
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

	mScene->simulate(frameTime);
	mScene->fetchResults(true);
}


void PhysxManager::onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
	
	Entidad* a = findEntityByPxActor(actor1);
	Entidad* b = findEntityByPxActor(actor2);
	if (a != b) 
	{
		a->OnCollisionEnter(b);
		if (a != nullptr && b != nullptr) b->OnCollisionEnter(a);
	}

	
}

void PhysxManager::onTrigger(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);

	Entidad* a = findEntityByPxActor(actor1);
	Entidad* b = findEntityByPxActor(actor2);
	if (a != b)
	{
		a->OnTriggerEnter(b);
		if(a != nullptr && b != nullptr) b->OnTriggerEnter(a);
	}

	std::cout << "TRIGGER COLLISION detected!\n";
}

// Copia los valores de un transform global a uno propio de physx
PxTransform PhysxManager::globalToPhysxTR(Transform& tr)
{
	PxTransform bodyTr = PxTransform();

	Vectola3D trPos = tr.getPosition();
	Quaterniola trRot = tr.getRotation();
	trRot.normalize();

	PxVec3 pos = PxVec3(trPos.getX(), trPos.getY(), trPos.getZ());
	bodyTr.p = pos;

	PxQuat rot = PxQuat(trRot.v.getX(), trRot.v.getY(), trRot.v.getZ(), trRot.s);
	bodyTr.q = rot;

	return bodyTr;
}

// Copia los valores de un transform tipico de physx a uno global
Transform PhysxManager::physxToGlobalTR(const PxRigidActor& body)
{
	Transform tr = Transform();

	PxTransform bodyTr = body.getGlobalPose();
	bodyTr.getNormalized();

	PxVec3 bodyPos = PxVec3(bodyTr.p);
	PxQuat bodyRot = PxQuat(bodyTr.q);

	Vectola3D pos = Vectola3D(bodyPos.x, bodyPos.y, bodyPos.z);
	tr.setPosition(pos);

	Quaterniola rot = Quaterniola(bodyRot.w, bodyRot.x, bodyRot.y, bodyRot.z);
	tr.setRotation(rot);

	return tr;
}

MOTOR_API Entidad* PhysxManager::findEntityByPxActor(PxActor* actor)
{
	RigidBody* body = nullptr;
	auto it = em().getAllEntidades().begin();
	while (it != em().getAllEntidades().end()) {
		Entidad* e = (*it).get();
		it++;

		if (e->getComponent<RigidBody>() == nullptr) continue;

		body = e->getComponent<RigidBody>();
		PxRigidDynamic* bodyRD = body->getBody();
		PxRigidStatic* bodyST = body->getStBody();

		PxActor* actorAUX = nullptr;
		if (bodyRD) actorAUX = dynamic_cast<PxActor*>(bodyRD);
		else if (bodyST) actorAUX = dynamic_cast<PxActor*>(bodyST);
		else continue;

		if (actorAUX && actorAUX == actor)
		{
			return e;
		}
	}
	return nullptr;
}

// Realiza la conversi�n de datos: Transform global --> PxTransform
void PhysxManager::setGlobalToPhysxTR(Entidad& e, PxRigidActor& body)
{
	Transform* tr = e.getComponent<Transform>();
	PxTransform bodyTR = globalToPhysxTR(*tr);
	body.setGlobalPose(bodyTR);
}

// Realiza la conversi�n de datos: PxTransform --> Transform global
void PhysxManager::setPhysxToGlobalTR(Entidad& e, PxRigidActor& body)
{
	Transform auxTR = physxToGlobalTR(body);
	Transform* tr = e.getComponent<Transform>();
	tr->setPosition(auxTR.getPosition());
	tr->setRotation(auxTR.getRotation());
}

// ---------------- FACTORY --------------------------------------------------

PxRigidDynamic* PhysxManager::createDynamic(const PxTransform& transform, const PxVec3& velocity)
{
	PxRigidDynamic* dynamic = physX()->createRigidDynamic(transform);
	dynamic->setLinearVelocity(velocity);
	mScene->addActor(*dynamic);
	return dynamic;
}

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

PxRigidStatic* PhysxManager::createStaticRigid(const PxTransform& transform)
{
	PxRigidStatic* body = physX()->createRigidStatic(transform);
	mScene->addActor(*body);
	return body;
}

PxRigidStatic* PhysxManager::createStaticRigid(const PxTransform& transform, const PxGeometry& geom, PxMaterial& material)
{
	PxShape* shape = createTriggerShape(geom, material, false);
	PxRigidStatic* body = createStaticRigid(transform, shape);
	shape->release(); // importante! (cuando ya se ha usado hay que borrarlo)
	return body;
}

PxRigidStatic* PhysxManager::createStaticRigid(const PxTransform& transform, PxShape* shape)
{
	PxRigidStatic* body = nullptr;

	if (shape)
	{
		body = mPhysics->createRigidStatic(transform);
		if(body) body->attachShape(*shape);
		if(body) mScene->addActor(*body);
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
	PxRigidBodyExt::updateMassAndInertia(*ball, 1000.f); //density = 1000 �arrasa con todo!
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

// ---------------- DEBUG --------------------------------------------------

void PhysxManager::debugTime()
{
	std::cout << "PCFreq : " << PCFreq << "\n";
	std::cout << "CounterStart : " << CounterStart << "\n";
	std::cout << "CounterLast : " << CounterLast << "\n";
	std::cout << "GetLastTime() = " << GetLastTime() << "\n";
	std::cout << "GlobalTimer : " << GlobalTimer << "\n";
}

void PhysxManager::debugBall()
{
	 
	// Comienza con una media de 15.05ms si se resetea con CounterLast = CounterStart,
	// pero su uso real es llevar el tiempo real de la app activa.
	if (GetLastTime() - GlobalTimer > 1.0) {
		PxVec3 v = testBALL->getGlobalPose().p;
		std::cout << "PhyBALL position : ";
		std::cout << "( " << v.x << " , " << v.y << " , " << v.z << " )" << "\n";
		GlobalTimer = GetLastTime();
	}
}

#include "RigidBody.h"
void PhysxManager::debugBuddy(Entidad* e)
{
	if (GetLastTime() - GlobalTimer > 2.0) {
		RigidBody* body = e->getComponent<RigidBody>();
		PxTransform tr = body->getBody()->getGlobalPose();
		std::cout << "Position RB = " << tr.p << "\n";
		std::cout << "Orientation RB = " << tr.q << "\n";
		GlobalTimer = GetLastTime();
	}
}

void PhysxManager::debugBody(PxRigidDynamic* rd)
{
	if (GetLastTime() - GlobalTimer > 2.0) {
		PxTransform tr = rd->getGlobalPose();
		std::cout << "Position RB = " << tr.p << "\n";
		std::cout << "Orientation RB = " << tr.q << "\n";
		GlobalTimer = GetLastTime();
	}
}

void PhysxManager::debugAllBodies()
{
	if (GetLastTime() - GlobalTimer > 2.0) {
		auto it = em().getAllEntidades().begin();
		while (it != em().getAllEntidades().end()) {
			Entidad* e = (*it).get();
			it++;
			if (e->getComponent<RigidBody>() == nullptr) continue;
			RigidBody* body = e->getComponent<RigidBody>();
			if (body->getBody()) {
				std::cout << "> Dynamic pos: " << body->getBody()->getGlobalPose().p << "\n";
				std::cout << "> Dynamic rot: " << body->getBody()->getGlobalPose().q << "\n";
			}
		}
	}
}

////----
float stepTime = 0.0f;

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