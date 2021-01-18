#include "pxPhysicsCharacterControllerInner.hpp"
#include "PhysXPhysicsManager.hpp"
#include"typedef.hpp"
const char* gDynamic = "Dynamic";
using namespace DirectX;
f32 constexpr GRAVITY = -50.f;
namespace Kumazuma::Client
{
	using namespace physx;
	pxPhysicsCharacterControllerInner::pxPhysicsCharacterControllerInner(std::weak_ptr<PhysXPhysicsManager>&& manager):
		manager_{std::move(manager)},
		gravityAccel_{0.f}
	{
	}
	pxPhysicsCharacterControllerInner::~pxPhysicsCharacterControllerInner()
	{
		if (auto manager = manager_.lock(); manager != nullptr)
		{
			manager->Delete(this);
		}
	}
	auto pxPhysicsCharacterControllerInner::SetCharacterController(physx::PxController* controller) -> void
	{
		controller_ = controller;
	}

	auto pxPhysicsCharacterControllerInner::PreUpdate(f32 timeDelta) -> void
	{
		PxControllerFilterCallback* cctFilterCallback{};
		PxFilterData const* filterData{};
		PxControllerFilters const filters{ filterData, this,  cctFilterCallback };
		f32 const  heightDelta{ jump_.GetAccel(GRAVITY, timeDelta) };
		f32 dy{};
		if (heightDelta != 0.f)
		{
			dy = heightDelta;
		}
		else
		{
			dy = gravityAccel_ + GRAVITY;
			gravityAccel_ += timeDelta * GRAVITY;
		}
		XMVECTOR vMoveDel{XMVectorSet(0.f, dy, 0.f, 0.f)};
		vMoveDel = vMoveDel + LoadF32x3(this->moveDelta_);
		vMoveDel *= timeDelta;
		auto const moveDelta{ StoreF32x3(vMoveDel) };
		PxVec3 disp{};
		disp.x = moveDelta.x;
		disp.y = moveDelta.y;
		disp.z = moveDelta.z;
		auto const flags{ controller_->move(disp, 0.f, timeDelta, filters, nullptr) };
		this->moveDelta_ = f32x3{};
		if (flags & PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			jump_.StopJump();
			gravityAccel_ = 0.f;
		}
	}

	auto pxPhysicsCharacterControllerInner::PostUpdate() -> void
	{
		this->GetComponent().OnPhysicsPostUpdate();
	}

	auto pxPhysicsCharacterControllerInner::GetPosition() -> DirectX::XMFLOAT3 
	{
		auto actor{ controller_->getActor() };
		auto transform{ actor->getGlobalPose() };
		auto const& position{ transform.p };
		return XMFLOAT3{ position.x, position.y, position.z };
	}
	auto pxPhysicsCharacterControllerInner::Move(DirectX::XMVECTOR moveDelta) -> void
	{
		XMVECTOR vDelta{ LoadF32x3(this->moveDelta_) };
		vDelta += moveDelta;
		this->moveDelta_ = StoreF32x3(vDelta);
	}
	auto pxPhysicsCharacterControllerInner::Jump(f32 speed) -> void
	{
		if (!this->jump_.jump_)
		{
			this->jump_.StartJump(speed);
		}
	}
	auto pxPhysicsCharacterControllerInner::IsJumping() const -> bool 
	{
		return jump_.jump_;
	}
	auto pxPhysicsCharacterControllerInner::preFilter(const physx::PxFilterData& filterData, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags) -> physx::PxQueryHitType::Enum
	{
		PX_UNUSED(actor);
		const char* actorName = shape->getActor()->getName();
		if (actorName == gDynamic)
			return PxQueryHitType::eNONE;

		return PxQueryHitType::eBLOCK;
	}
	auto pxPhysicsCharacterControllerInner::postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit) -> physx::PxQueryHitType::Enum
	{
		return physx::PxQueryHitType::eBLOCK;
	}
	auto pxPhysicsCharacterControllerInner::getBehaviorFlags(physx::PxShape const& shape, physx::PxActor const& actor) -> physx::PxControllerBehaviorFlags
	{
		return PxControllerBehaviorFlags(0);
	}
	auto pxPhysicsCharacterControllerInner::getBehaviorFlags(physx::PxController const& controller) -> physx::PxControllerBehaviorFlags
	{
		return PxControllerBehaviorFlags(0);
	}
	auto pxPhysicsCharacterControllerInner::getBehaviorFlags(physx::PxObstacle const& obstacle) -> physx::PxControllerBehaviorFlags
	{
		return PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT | PxControllerBehaviorFlag::eCCT_SLIDE;
	}
}

