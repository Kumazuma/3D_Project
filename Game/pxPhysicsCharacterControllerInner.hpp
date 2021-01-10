#pragma once
#include<PxPhysicsAPI.h>
#include"PhysicsCharacterController.hpp"
#include"pxJump.hpp"
namespace Kumazuma::Client
{
	class PhysXPhysicsManager;
	class pxPhysicsCharacterControllerInner:
		public PhysicsCharacterControllerInner,
		public physx::PxQueryFilterCallback,
		public physx::PxControllerBehaviorCallback

	{
	public:
		pxPhysicsCharacterControllerInner(std::weak_ptr<PhysXPhysicsManager>&& manager);
		~pxPhysicsCharacterControllerInner();
		auto SetCharacterController(physx::PxController* controller) ->void;
		auto PreUpdate(f32 timeDelta)->void;
		auto PostUpdate()->void;
	public:
		auto GetPosition()->DirectX::XMFLOAT3 override;
		auto Move(DirectX::XMVECTOR moveDelta)->void override;
	private:
		auto preFilter(
			physx::PxFilterData const& filterData,
			physx::PxShape const* shape,
			physx::PxRigidActor const* actor,
			physx::PxHitFlags& queryFlags)
			->physx::PxQueryHitType::Enum override;
		auto postFilter(
			physx::PxFilterData const& filterData,
			physx::PxQueryHit const& hit)
			->physx::PxQueryHitType::Enum override;
		///////////////////////////////////////////////////////////////////////////////
		// Implements PxControllerBehaviorCallback
		auto getBehaviorFlags(
			physx::PxShape const& shape,
			physx::PxActor const& actor)
			->physx::PxControllerBehaviorFlags override;
		auto getBehaviorFlags(
			physx::PxController const& controller)
			->physx::PxControllerBehaviorFlags override;
		auto getBehaviorFlags(
			physx::PxObstacle const& obstacle)
			->physx::PxControllerBehaviorFlags override;
	private:
		std::weak_ptr<PhysXPhysicsManager> manager_;
		f32x3 moveDelta_;
		f32 gravityAccel_;
		Jump jump_;
		physx::PxController* controller_;
	};
}