#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include <PxPhysicsAPI.h>
using namespace physx;

physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);

class ContactReportCallback : public PxSimulationEventCallback
{
	void onConstraintBreak(PxConstraintInfo* , PxU32 ) { printf("onConstraintBreak\n"); }
	void onWake(PxActor** , PxU32 ) { printf("onWake\n"); }
	void onSleep(PxActor** , PxU32 ) { printf("onSleep\n"); }
	void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) { printf("onAdvance\n"); }

	// detecta colisiones de shape clasificado como trigger
	void onTrigger(PxTriggerPair* pairs, PxU32 count);

	// detecta colisiones normales
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 count);

	bool isTriggerShape(PxShape* shape)
	{
		// Detects native built-in triggers.
		if (shape->getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
			return true;
	}
};

#endif