/*!*************************************************************************
****
\file boxCollider.cpp
\author Go Ruo Yan
\par DP email: ruoyan.go@digipen.edu
\date 21-10-2023
\brief  This program defines the functions in the Box Collider component
		class
****************************************************************************
***/
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include "components/boxCollider.h"

RTTR_REGISTRATION
{
	using namespace TDS;

	rttr::registration::class_<BoxCollider>("Box Collider")
		.method("GetIsTrigger", &BoxCollider::GetIsTrigger)
		.method("SetIsTrigger", &BoxCollider::SetIsTrigger)
		.property("IsTrigger", &BoxCollider::mIsTrigger)
		.method("GetCenter", &BoxCollider::GetCenter)
		.method("SetCenter", rttr::select_overload<void(Vec3)>(&BoxCollider::SetCenter))
		.method("SetCenter", rttr::select_overload<void(float, float, float)>(&BoxCollider::SetCenter))
		.property("Center", &BoxCollider::mCenter)
		.method("GetSize", &BoxCollider::GetSize)
		.method("SetSize", rttr::select_overload<void(Vec3)>(&BoxCollider::SetSize))
		.method("SetSize", rttr::select_overload<void(float, float, float)>(&BoxCollider::SetSize))
		.property("Size", &BoxCollider::mSize);
}

namespace TDS
{
	/*!*************************************************************************
	Initializes the Collider component when created
	****************************************************************************/
	BoxCollider::BoxCollider() : mIsTrigger(false),
		mCenter(Vec3(0.0f, 0.0f, 0.0f)),
		mSize(Vec3(1.0f, 1.0f, 1.0f))
	{
		//half extents are all half of size
		JPH::Vec3 halfextents = JPH::Vec3(mSize.x * 0.5f, mSize.y * 0.5f, mSize.z * 0.5f);
		//CreateJPHBoxCollider(halfextents, JPH::cDefaultConvexRadius);
	}

	/*!*************************************************************************
	Initializes the Collider component when created, given another Collider
	component to move (for ECS)
	****************************************************************************/
	BoxCollider::BoxCollider(BoxCollider&& toMove) noexcept : mIsTrigger(toMove.mIsTrigger),
		mCenter(toMove.mCenter),
		mSize(toMove.mSize)
	{ }

	BoxCollider* GetBoxCollider(EntityID entityID)
	{
		return ecs.getComponent<BoxCollider>(entityID);
	}

	/*!***********************************************************************
		Takes values from RTTR or updated values from imGui and uses it to construct
		a JPH Box Shape
	***************************************************************************/
	JPH::BoxShape& BoxCollider::CreateJPHBoxCollider(JPH::Vec3 inHalfExrent, float inConvexRadius)
	{

		JPH::BoxShape result = JPH::BoxShape(inHalfExrent, inConvexRadius, nullptr);
		std::cout << "JPH Box created with halfextent: " << inHalfExrent
			<< " and radius: " << inConvexRadius <<
			std::endl;
		return result;
	}

	/*!***********************************************************************
		Takes values from RTTR or updated values from imGui and uses it to construct
		a JPH Box Shape
	***************************************************************************/
	JPH::BoxShape& BoxCollider::CreateJPHBoxCollider(JPH::BoxShape& input)
	{
		JPH::BoxShape result = JPH::BoxShape(input.GetHalfExtent() , input.GetConvexRadius(), nullptr);
		std::cout << "JPH Box created with halfextent: " << input.GetHalfExtent()
			<< " and radius: " << input.GetConvexRadius() <<
			std::endl;
		return result;
	}
}