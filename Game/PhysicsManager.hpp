#pragma once
#include<memory>
#include<unordered_map>
#include<StringLiteral.h>
#include"typedef.hpp"
#include<unordered_set>
#include<SpinLock.hpp>
#include<vector>
#include <Game/component.hpp>
#include <optional>
#include<DirectXMath.h>
#include <Game/event.hpp>
#include "PhysicsCharacterController.hpp"
#include "SimpleTransform.hpp"
class WavefrontOBJMesh;
namespace Kumazuma::Game
{
	class Object;
}

namespace Kumazuma::Client
{
	class PhysicsComponent;
	class PhysicsCharacterController;
	class PhysicsManager: public std::enable_shared_from_this<PhysicsManager>
	{
		PhysicsManager(PhysicsManager const&) = delete;
		PhysicsManager(PhysicsManager&& rhs) = delete;
	protected:
		PhysicsManager();
	public:
		virtual ~PhysicsManager();
		static auto Initialize()->void;
		static auto Instance()->std::shared_ptr<PhysicsManager>;
		static auto Release()->void;
		virtual auto SetCharacterColliderBox(StringLiteral<wchar_t> const& colliderKey, f32 width, f32 height, f32 depth)->void = 0;
		virtual auto SetCharacterColliderSphere(StringLiteral<wchar_t> const& colliderKey, f32 radius)->void = 0;
		virtual auto SetCharacterColliderCylinder(StringLiteral<wchar_t> const& colliderKey, f32 width, f32 height, f32 depth)->void = 0;
		virtual auto SetCharacterColliderCapsule(StringLiteral<wchar_t> const& colliderKey, f32 radius, f32 height)->void = 0;
		virtual auto SetMap(std::vector<std::unique_ptr< WavefrontOBJMesh> >const& meshs, std::vector< SimpleTransform> const& )->void = 0;
		virtual auto Update(float timeDelta)->void = 0;
		virtual auto CreatePhysicsComponent(StringLiteral<wchar_t> colliderKey, f32 mass, DirectX::XMFLOAT3 const& initialPosition, DirectX::XMFLOAT3 const& offset) ->std::optional<PhysicsComponent> = 0;
		virtual auto CreateCharacterController(f32 radius, f32 height, DirectX::XMFLOAT3 const& initialPosition, DirectX::XMFLOAT3 const& offset)->std::optional<PhysicsCharacterController> = 0;
	private:
		//auto RemovePhysicsComponent(btKinematicCharacterController*)->void;
		SpinLock spinLocker_;
		static SpinLock s_instanceChecker;
		static std::shared_ptr<PhysicsManager> s_instance;
	};
	extern Game::EventTag<Game::Event> const EVT_PHYSICS_POST_UPDATE;
	class PhysicsComponent : public Game::Component
	{
		friend class PhysicsManager;
	public:
		static Game::ComponentTag<PhysicsComponent> const TAG;
		PhysicsComponent();
		PhysicsComponent(PhysicsComponent const&) = delete;
		PhysicsComponent(PhysicsComponent&& rhs) noexcept;
		~PhysicsComponent();
		auto Clone()const->Game::Component* override;
		//auto GetCharacterContoller()->btKinematicCharacterController*;
	protected:
		auto OnPhysicsPostUpdate(Game::Event const& evt)->void;
	protected:
		std::weak_ptr< PhysicsManager> physicsManager_;
		DirectX::XMFLOAT3A offset_;
		//btKinematicCharacterController* collisionObj_;
		//auto(PhysicsManager::*OnCOMDelete)(btKinematicCharacterController*)->void;
	};

}