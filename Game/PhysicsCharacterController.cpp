#include"PhysicsCharacterController.hpp"
#include"PhysicsManager.hpp"
#include<game/object.hpp>
#include<game/TransformComponent.hpp>
#undef GetObject
using namespace DirectX;
namespace Kumazuma::Client
{
	Game::ComponentTag<PhysicsCharacterController> const PhysicsCharacterController::TAG{ "PhysicsCharacterController" };
	
	PhysicsCharacterController::PhysicsCharacterController(PhysicsCharacterControllerInner* cc, DirectX::XMFLOAT3 const& offset):
		Component{ TAG },
		inner_{ cc },
		offset_{offset}
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
	auto PhysicsCharacterController::OnPhysicsPostUpdate() -> void
	{
		auto vPos{ LoadF32x3(inner_->GetPosition()) };
		auto vOffset{ LoadF32x3(this->offset_) };
		auto position{ StoreF32x3(vPos + vOffset) };
		if (auto obj = GetObj().lock(); obj != nullptr)
		{
			obj->GetComponent<Game::TransformComponent>()->SetPosition(position);
		}
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