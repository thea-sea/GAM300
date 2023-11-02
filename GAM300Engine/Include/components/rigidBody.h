/*!*************************************************************************
****
\file rigidBody.h
\author Go Ruo Yan
\par DP email: ruoyan.go@digipen.edu
\date 28-9-2023
\brief  This program declares the functions in the rigidBody component class
****************************************************************************
***/

#ifndef RIGIDBODYCOMPONENT
#define RIGIDBODYCOMPONENT

#include "Vector3.h"
#include "ecs/ecs.h"

namespace TDS
{
	class RigidBody : public IComponent
	{
	public:
		enum class MotionType //EMotionType in Jolt/.../MotionType.h
		{
			NONE,		//when type is not set
			STATIC,		///< Non movable
			KINEMATIC,  ///< Movable using velocities only, does not respond to forces
			DYNAMIC		///< Responds to forces as a normal physics object
		};

		/*!*************************************************************************
		Initializes the RigidBody component when created
		****************************************************************************/
		DLL_API RigidBody();
		/*!*************************************************************************
		Initializes the RigidBody component when created, given another RigidBody
		component to move (for ECS)
		****************************************************************************/
		DLL_API RigidBody(RigidBody&& toMove) noexcept;
		/*!*************************************************************************
		Destructor for the RigidBody component class
		****************************************************************************/
		DLL_API ~RigidBody() = default;
		/*!*************************************************************************
		Getter and setter functions for the variables in the RigidBody component class
		****************************************************************************/
		DLL_API float& GetMass() { return mMass; }
		DLL_API void SetMass(float mass) { mMass = mass; }

		DLL_API Vec3& GetAcceleration() { return mAcceleration; }
		DLL_API void SetAcceleration(Vec3 acceleration) { mAcceleration = acceleration; }

		DLL_API Vec3& GetLinearVel() { return mLinearVelocity; }
		DLL_API void SetLinearVel(Vec3 velocity) { mLinearVelocity = velocity; }
		DLL_API void SetLinearVel(float velocityX, float velocityY, float velocityZ) { mLinearVelocity = Vec3(velocityX, velocityY, velocityZ); }

		DLL_API Vec3& GetAngularVel() { return mAngularVelocity; }
		DLL_API void SetAngularVel(Vec3 velocity) { mAngularVelocity = velocity; }
		DLL_API void SetAngularVel(float velocityX, float velocityY, float velocityZ) { mAngularVelocity = Vec3(velocityX, velocityY, velocityZ); }

		DLL_API Vec3& GetInputForce() { return mInputForce; }
		DLL_API void SetInputForce(Vec3 inputForce) { mInputForce = inputForce; }
		DLL_API void SetInputForce(float inputForceX, float inputForceY, float inputForceZ) { mInputForce = Vec3(inputForceX, inputForceY, inputForceZ); }

		DLL_API Vec3& GetNormalizedForce() { return mNormalizedForce; }
		DLL_API void SetNormalizedForce(Vec3 normalizedForce) { mNormalizedForce = normalizedForce; }
		DLL_API void SetNormalizedForce(float normalizedForceX, float normalizedForceY, float normalizedForceZ) { mNormalizedForce = Vec3(normalizedForceX, normalizedForceY, normalizedForceZ); }

		DLL_API Vec3& GetDirection() { return mDirection; }
		DLL_API void SetDirection(Vec3 direction) { mDirection = direction; }
		DLL_API void SetDirection(float directionX, float directionY, float directionZ) { mDirection = Vec3(directionX, directionY, directionZ); }

		DLL_API Vec3& GetNextPosition() { return mNextPosition; }
		DLL_API void SetNextPosition(Vec3 nextPosition) { mNextPosition = nextPosition; }

		DLL_API float& GetFriction() { return mFriction; }
		DLL_API void SetFrictionCoefficient(float friction) { mFriction = friction; }

		DLL_API float& GetRestitution() { return mRestitution; }
		DLL_API void SetRestitution(float restitution) { mRestitution = restitution; }

		DLL_API float& GetInverseMass() { return mInverseMass; }
		DLL_API void setInverseMass(float mass) { mInverseMass = 1.0f / mass; }

		DLL_API float& GetGravity() { return mGravity; }
		DLL_API void SetGravity(float gravity) { mGravity = gravity; }
		
		DLL_API MotionType& GetMotionType() { return mMotionType; }
		DLL_API void SetMotionType(MotionType motionType) { mMotionType = motionType; }

		DLL_API uint32_t GetBodyID() { return mBodyID; }
		DLL_API void SetBodyID(uint32_t bodyID) { mBodyID = bodyID; }

		void UpdateMotionType();
		void SetAllFalse();

		RTTR_ENABLE(IComponent);
		RTTR_REGISTRATION_FRIEND

	public:
		bool isStatic, isKinematic, isDynamic;
		static inline const char * current_motion_type;


	private:
		float mMass;

		Vec3 mAcceleration;
		Vec3 mLinearVelocity;
		Vec3 mAngularVelocity;
		Vec3 mDirection;
		Vec3 mNextPosition;
		Vec3 mInputForce;
		Vec3 mNormalizedForce;
		Vec3 mTotalForce;

		float mFriction;
		float mRestitution;
		float mInverseMass;
		float mGravity;
		
		MotionType	mMotionType;
		uint32_t	mBodyID;
	};

}

#endif //RIGIDBODYCOMPONENT