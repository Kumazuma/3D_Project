#include"PhysicsCharacterController.hpp"
#include"PhysicsManager.hpp"
#include<game/object.hpp>
#include<game/TransformComponent.hpp>
#undef GetObject
using namespace DirectX;
namespace Kumazuma::Client
{
	IMPLEMENT_COMPONENT_CLASS(Kumazuma::Client::PhysicsCharacterController)

	PhysicsCharacterController::PhysicsCharacterController(PhysicsCharacterControllerInner* cc, DirectX::XMFLOAT3 const& offset, f32 radius, f32 height):
		Component{ TAG },
		inner_{ cc },
		offset_{offset},
		radius_{radius},
		height_{height}
	{
		inner_->SetComponent(this);
	}

	PhysicsCharacterController::PhysicsCharacterController(PhysicsCharacterController&& rhs) noexcept :
		Game::Component{ rhs },
		offset_{rhs.offset_}
	{
		inner_ = std::move(rhs.inner_);
		inner_->SetComponent(this);
	}
	PhysicsCharacterController::~PhysicsCharacterController()
	{

	}
	auto PhysicsCharacterController::Move(DirectX::XMVECTOR dir) -> void
	{
		inner_->Move(dir);
	}
	auto PhysicsCharacterController::Stop() -> void
	{
	}
	auto PhysicsCharacterController::Jump(f32 speed) -> void
	{
		inner_->Jump(speed);
	}
	auto PhysicsCharacterController::IsJumping() const -> bool
	{
		return inner_->IsJumping();
	}
	auto PhysicsCharacterController::OnPhysicsPostUpdate() -> void
	{

	}
	auto PhysicsCharacterController::GetPosition() const -> f32x3 
	{
		auto vPos{ LoadF32x3(inner_->GetPosition()) };
		auto vOffset{ LoadF32x3(this->offset_) };
		return StoreF32x3(vPos + vOffset);
	}
	auto PhysicsCharacterController::GetOffset() const -> f32x3
	{
		return offset_;
	}
	auto PhysicsCharacterController::SetOffset(f32x3 const& offset) -> void
	{
		offset_ = offset;
	}
	auto PhysicsCharacterController::GetRadius() const -> f32
	{
		return radius_;
	}
	auto PhysicsCharacterControllerInner::SetComponent(PhysicsCharacterController* com) -> void
	{
		com_ = com;
	}
	auto PhysicsCharacterControllerInner::GetOffset() -> DirectX::XMFLOAT3 const&
	{
		return com_->offset_;
	}
	auto PhysicsCharacterControllerInner::GetComponent() -> PhysicsCharacterController&
	{
		return *com_;
	}
	auto PhysicsCharacterControllerInner::GetObject() -> std::weak_ptr<Game::Object>
	{
		return com_->GetObj();
	}
}