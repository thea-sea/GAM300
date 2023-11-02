/*!*************************************************************************
****
\file boxCollider.h
\author Go Ruo Yan
\par DP email: ruoyan.go@digipen.edu
\date 21-10-2023
\brief  This program declares the functions in the Box Collider component 
		class
****************************************************************************
***/

#ifndef BOXCOLLIDERCOMPONENT
#define BOXCOLLIDERCOMPONENT

#include "Vector3.h"
#include "ecs/ecs.h"


namespace JPH //forward declare
{
	class BoxShape;
	class Vec3;

}

namespace TDS
{

	class BoxCollider : public IComponent
	{
	public:
		/*!*************************************************************************
		Initializes the Collider component when created
		****************************************************************************/
		DLL_API BoxCollider();
		/*!*************************************************************************
		Initializes the Collider component when created, given another Collider
		component to move (for ECS)
		****************************************************************************/
		DLL_API BoxCollider(BoxCollider&& toMove) noexcept;
		/*!*************************************************************************
		Destructor for the Collider component class
		****************************************************************************/
		DLL_API ~BoxCollider() = default;
		/*!*************************************************************************
		Getter and setter functions for the variables in the Collider component class
		****************************************************************************/
		DLL_API bool& GetIsTrigger() { return mIsTrigger; }
		DLL_API void SetIsTrigger(bool isTrigger) { mIsTrigger = isTrigger; }

		DLL_API Vec3& GetCenter() { return mCenter; }
		DLL_API void SetCenter(Vec3 center) { mCenter = center; }
		DLL_API void SetCenter(float x, float y, float z) { mCenter = Vec3(x, y, z); }

		DLL_API Vec3& GetSize() { return mSize; }
		DLL_API void SetSize(Vec3 size) { mSize = size; }
		DLL_API void SetSize(float x, float y, float z) { mSize = Vec3(x, y, z); }

		//for Jolt Physics Capsule Collider creation
		DLL_API JPH::BoxShape& CreateJPHBoxCollider(JPH::Vec3 inHalfExrent, float inConvexRadius);
		DLL_API JPH::BoxShape& CreateJPHBoxCollider(JPH::BoxShape& input);

		RTTR_ENABLE(IComponent);
		RTTR_REGISTRATION_FRIEND

	private:
		bool mIsTrigger;
		//Material mMaterial;
		Vec3 mCenter;
		Vec3 mSize;
	};

	DLL_API BoxCollider* GetBoxCollider(EntityID entityID);
}

#endif
