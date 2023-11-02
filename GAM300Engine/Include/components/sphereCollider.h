/*!*************************************************************************
****
\file capsuleCollider.h
\author Go Ruo Yan
\par DP email: ruoyan.go@digipen.edu
\date 21-10-2023
\brief  This program declares the functions in the Capsule Collider
		component class
****************************************************************************
***/

#ifndef SPHERECOLLIDERCOMPONENT
#define SPHERECOLLIDERCOMPONENT

#include "Vector3.h"
#include "ecs/ecs.h"

namespace JPH
{
	class SphereShape;
}

namespace TDS
{
	class SphereCollider : public IComponent
	{
	public:
		/*!*************************************************************************
		Initializes the Collider component when created
		****************************************************************************/
		DLL_API SphereCollider();
		/*!*************************************************************************
		Initializes the Collider component when created, given another Collider
		component to move (for ECS)
		****************************************************************************/
		DLL_API SphereCollider(SphereCollider&& toMove) noexcept;
		/*!*************************************************************************
		Destructor for the Collider component class
		****************************************************************************/
		DLL_API ~SphereCollider() = default;
		/*!*************************************************************************
		Getter and setter functions for the variables in the Collider component class
		****************************************************************************/
		DLL_API bool& GetIsTrigger() { return mIsTrigger; }
		DLL_API void SetIsTrigger(bool isTrigger) { mIsTrigger = isTrigger; }

		DLL_API Vec3& GetCenter() { return mCenter; }
		DLL_API void SetCenter(Vec3 center) { mCenter = center; }
		DLL_API void SetCenter(float x, float y, float z) { mCenter = Vec3(x, y, z); }

		DLL_API float& GetRadius() { return mRadius; }
		DLL_API void SetRadius(float radius) { mRadius = radius; }

		RTTR_ENABLE(IComponent);
		RTTR_REGISTRATION_FRIEND

		//for Jolt Physics Capsule Collider creation
		DLL_API JPH::SphereShape& CreateJPHSphereCollider(float inRadius);
		DLL_API JPH::SphereShape& CreateJPHSphereCollider(JPH::SphereShape& input);

	private:
		bool mIsTrigger;
		//Material mMaterial;
		Vec3 mCenter;
		float mRadius;
	};

	DLL_API SphereCollider* GetSphereCollider(EntityID entityID);
}

#endif
