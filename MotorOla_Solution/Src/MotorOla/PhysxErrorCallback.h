#ifndef PHYSICS_ERROR_CALLBACK_H
#define PHYSICS_ERROR_CALLBACK_H

#include "foundation/PxErrorCallback.h"
#include "PxPhysXConfig.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

	/**
	\brief default implementation of the error callback
	*/

	class PhysxErrorCallback : public PxErrorCallback
	{
	public:
		PhysxErrorCallback();
		~PhysxErrorCallback();

		virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line);
	};

#if !PX_DOXYGEN
} // namespace physx
#endif

#endif
