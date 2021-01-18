#pragma once
#include<game/component.hpp>
#include<DirectXMath.h>
#include"typedef.hpp"
#undef GetObject
namespace Kumazuma::Client
{
	class PhysicsManager;
	class PhysicsCharacterControllerInner;
	class PhysicsCharacterController : public Game::Component
	{
		friend class PhysicsManager;
		friend class PhysicsCharacterControllerInner;
	
	public:
		PhysicsCharacterController(PhysicsCharacterControllerInner* cc, DirectX::XMFLOAT3 const& offset, f32 radius, f32 height);
		PhysicsCharacterController(PhysicsCharacterController&& rhs) noexcept;
		~PhysicsCharacterController();
		auto Clone()const->Game::Component* override { return nullptr; }
		auto Move(DirectX::XMVECTOR dir)->void;
		auto Stop()->void;
		auto Jump(f32 speed)->void;
		auto IsJumping()const->bool;
		auto OnPhysicsPostUpdate()->void;
		auto GetPosition()const->f32x3 ;
		auto GetOffset()const->f32x3;
		auto SetOffset(f32x3 const& offset)->void;
		auto GetRadius()const->f32;
	private:
		std::unique_ptr<PhysicsCharacterControllerInner> inner_;
		f32 radius_;
		f32 height_;
		DirectX::XMFLOAT3 offset_;

		DECLARE_COMPONENT_CLASS(Kumazuma::Client::PhysicsCharacterController)
	};
	class PhysicsCharacterControllerInner
	{
	public:
		virtual ~PhysicsCharacterControllerInner() = default;
		virtual auto Move(DirectX::XMVECTOR moveDelta)->void = 0;
		virtual auto GetPosition()->DirectX::XMFLOAT3 = 0;
		virtual auto Jump(f32 speed)->void = 0;
		virtual auto IsJumping()const->bool = 0;
		auto SetComponent(PhysicsCharacterController* com)->void;
	protected:
		auto GetOffset()->DirectX::XMFLOAT3 const&;
		auto GetComponent()->PhysicsCharacterController&;
		auto GetObject()->std::weak_ptr<Game::Object>;
	private:
		PhysicsCharacterController* com_;
	};
}