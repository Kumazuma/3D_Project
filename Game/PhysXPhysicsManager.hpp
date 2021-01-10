#pragma once
#include <PxPhysicsAPI.h>
#include <memory>
#include <StringLiteral.h>
#include <vector>
#include "typedef.hpp"
#include <optional>
#include "PhysicsManager.hpp"
namespace Kumazuma::Client
{
	class pxPhysicsCharacterControllerInner;
	class PhysXPhysicsManager :
		//PxQueryFilterCallback
		public PhysicsManager,
		public physx::PxSimulationEventCallback,
		public physx::PxUserControllerHitReport
	{
		friend class PhysicsManager;
		PhysXPhysicsManager(PhysXPhysicsManager const&) = delete;
		PhysXPhysicsManager(PhysXPhysicsManager&& rhs) = delete;
		PhysXPhysicsManager();
	public:
		~PhysXPhysicsManager();
		auto SetCharacterColliderBox(StringLiteral<wchar_t> const& colliderKey, f32 width, f32 height, f32 depth)->void;
		auto SetCharacterColliderSphere(StringLiteral<wchar_t> const& colliderKey, f32 radius)->void;
		auto SetCharacterColliderCylinder(StringLiteral<wchar_t> const& colliderKey, f32 width, f32 height, f32 depth)->void;
		auto SetCharacterColliderCapsule(StringLiteral<wchar_t> const& colliderKey, f32 radius, f32 height)->void;
		auto SetMap(std::vector<std::unique_ptr< WavefrontOBJMesh> >const& meshs)->void;
		auto Update(f32 timeDelta)->void;
		auto CreatePhysicsComponent(StringLiteral<wchar_t> colliderKey, f32 mass, DirectX::XMFLOAT3 const& initialPosition, DirectX::XMFLOAT3 const& offset)->std::optional<PhysicsComponent>;
		auto CreateCharacterController(f32 radius, f32 height, DirectX::XMFLOAT3 const& initialPosition, DirectX::XMFLOAT3 const& offset)->std::optional<PhysicsCharacterController>;
		auto Delete(pxPhysicsCharacterControllerInner* inner)->void;
	private:
		auto onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)->void override;
		auto onWake(physx::PxActor** actors, physx::PxU32 count)->void override;
		auto onSleep(physx::PxActor** actors, physx::PxU32 count)->void override;
		auto onContact(physx::PxContactPairHeader const& pairHeader, physx::PxContactPair const* pairs, physx::PxU32 nbPairs)->void override;
		auto onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)->void override;
		auto onAdvance(physx::PxRigidBody const* const* bodyBuffer, physx::PxTransform const* poseBuffer, const physx::PxU32 count)->void override;
	private:
		// Implements PxUserControllerHitReport
		auto onShapeHit(const physx::PxControllerShapeHit& hit)->void override;
		auto onControllerHit(const physx::PxControllersHit& hit)->void override {}
		auto onObstacleHit(const physx::PxControllerObstacleHit& hit)->void override {}
	private:


	private:
		static auto FilterShader(
			physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize
		)->physx::PxFilterFlags;
	private:
		//auto RemovePhysicsComponent(btKinematicCharacterController*)->void;
		SpinLock spinLocker_;

		physx::PxDefaultAllocator allocator;
		physx::PxDefaultErrorCallback errorCallback;
		physx::PxFoundation* foundation;
		physx::PxPhysics* physics;
		physx::PxDefaultCpuDispatcher* dispatcher;
		physx::PxPvd* pvd;
		physx::PxScene* scene;
		physx::PxCooking* cooking;
		physx::PxControllerManager* controllerManager;

		std::vector<pxPhysicsCharacterControllerInner*> charContollers_;
		std::vector <physx::PxTriangleMesh*> mapTriangleMeshs;
		std::vector <physx::PxRigidActor*> mapMeshRigids;
		std::unordered_map < StringLiteral<wchar_t>, physx::PxShape* > shapes;
		std::unordered_map < StringLiteral<wchar_t>, physx::PxMaterial*> materials;


	};
}