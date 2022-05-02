#include "ContactReportCallback.h"
#include "PhysxManager.h"

//extern void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	// all initial and persisting reports for everything, with per-point data
	pairFlags = physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT
		| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
		| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
	return physx::PxFilterFlag::eDEFAULT;
}

void ContactReportCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	//PX_UNUSED(pairs);
	//PX_UNUSED(nbPairs);
	printf("onContact: %d pairs\n", nbPairs);

	while (nbPairs--)
	{
		const PxContactPair& current = *pairs++;

		// The reported pairs can be trigger pairs or not. We only enabled contact reports for
		// trigger pairs in the filter shader, so we don't need to do further checks here. In a
		// real-world scenario you would probably need a way to tell whether one of the shapes
		// is a trigger or not. You could e.g. reuse the PxFilterData like we did in the filter
		// shader, or maybe use the shape's userData to identify triggers, or maybe put triggers
		// in a hash-set and test the reported shape pointers against it. Many options here.

		if (current.events & (PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_CCD))
			printf("Shape is entering trigger volume\n");
		if (current.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
			printf("Shape is leaving trigger volume\n");

		if (isTriggerShape(current.shapes[0]) && isTriggerShape(current.shapes[1]))
			printf("Trigger-trigger overlap detected\n");
	}	

	physx::PxActor* actor1 = pairHeader.actors[0];
	physx::PxActor* actor2 = pairHeader.actors[1];
	pm().onCollision(actor1, actor2);
}

void ContactReportCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	printf("onTrigger: %d trigger pairs\n", count);
	
	while (count--)
	{
		const PxTriggerPair& current = *pairs++;
		if (current.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			printf("Shape is entering trigger volume\n");
		if (current.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			printf("Shape is leaving trigger volume\n");

		physx::PxActor* actor1 = current.otherActor;
		physx::PxActor* actor2 = current.triggerActor;
		pm().onTrigger(actor1, actor2);
	}
}