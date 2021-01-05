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
namespace Kumazuma::Game
{
	class Object;
}
class WowMapMeshObject;
class btKinematicCharacterController;
namespace Kumazuma::Client
{
	class PhysicsComponent;
	class PhysicsManager
	{
		struct Bullet;
		PhysicsManager(PhysicsManager const&) = delete;
		PhysicsManager(PhysicsManager&& rhs) = delete;
		PhysicsManager();
	public:
		~PhysicsManager();
		static auto Initialize()->void;
		static auto Instance()->std::shared_ptr<PhysicsManager>;
		static auto Release()->void;
		auto SetCharacterColliderBox(StringLiteral<wchar_t> colliderKey, f32 width, f32 height, f32 depth)->void;
		auto SetCharacterColliderSphere(StringLiteral<wchar_t> colliderKey, f32 radius)->void;
		auto SetCharacterColliderCylinder(StringLiteral<wchar_t> colliderKey, f32 width, f32 height, f32 depth)->void;
		auto SetCharacterColliderCapsule(StringLiteral<wchar_t> colliderKey, f32 radius, f32 height)->void;
		auto SetMap(std::vector<std::unique_ptr< WowMapMeshObject> >const& meshs)->void;
		auto Update(float timeDelta)->void;
		auto CreatePhysicsComponent(StringLiteral<wchar_t> colliderKey, f32 mass, DirectX::XMFLOAT3 const& initialPosition, DirectX::XMFLOAT3 const& offset) ->std::optional<PhysicsComponent>;

	private:
		auto RemovePhysicsComponent(btKinematicCharacterController*)->void;
		SpinLock spinLocker_;
		Bullet* bullet_;
		std::unordered_set< std::shared_ptr<Game::Object> > characters_;
		
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
		auto GetCharacterContoller()->btKinematicCharacterController*;
	protected:
		auto OnPhysicsPostUpdate(Game::Event const& evt)->void;
	protected:
		std::weak_ptr< PhysicsManager> physicsManager_;
		DirectX::XMFLOAT3A offset_;
		btKinematicCharacterController* collisionObj_;
		auto(PhysicsManager::*OnCOMDelete)(btKinematicCharacterController*)->void;
	};
}