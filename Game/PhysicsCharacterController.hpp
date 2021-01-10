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
		static Game::ComponentTag<PhysicsCharacterController> const TAG;
	private:
	public:
		PhysicsCharacterController(PhysicsCharacterControllerInner* cc, DirectX::XMFLOAT3 const& offset);
		PhysicsCharacterController(PhysicsCharacterController&& rhs) noexcept;
		~PhysicsCharacterController();
		auto Clone()const->Game::Component* override { return nullptr; }
		auto Move(DirectX::XMVECTOR dir)->void;
		auto Stop()->void;
		auto OnPhysicsPostUpdate()->void;
	private:
		std::unique_ptr<PhysicsCharacterControllerInner> inner_;
		DirectX::XMFLOAT3 offset_;
	};
	class PhysicsCharacterControllerInner
	{
	public:
		virtual ~PhysicsCharacterControllerInner() = default;
		virtual auto Move(DirectX::XMVECTOR moveDelta)->void = 0;
		virtual auto GetPosition()->DirectX::XMFLOAT3 = 0;
		auto SetComponent(PhysicsCharacterController* com)->void;
	protected:
		auto GetOffset()->DirectX::XMFLOAT3 const&;
		auto GetComponent()->PhysicsCharacterController&;
		auto GetObject()->std::weak_ptr<Game::Object>;
	private:
		PhysicsCharacterController* com_;
	};
}