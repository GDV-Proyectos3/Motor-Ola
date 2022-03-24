#pragma once

#include <PxPhysicsAPI.h>
void initPhysics(bool interactive);
void stepPhysics(bool interactive, double t);
void cleanupPhysics(bool interactive);
void keyPress(unsigned char key, const physx::PxTransform& camera);
void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);
int snippetMain(int, const char* const*);
