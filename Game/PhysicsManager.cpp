#include "PhysicsManager.hpp"
#include <mutex>
#include "PhysXPhysicsManager.hpp"
namespace Kumazuma::Client
{
	Game::EventTag<Game::Event> const EVT_PHYSICS_POST_UPDATE{ "EVT_PHYSICS_POST_UPDATE" };

	SpinLock PhysicsManager::s_instanceChecker = SpinLock{};
	std::shared_ptr< PhysicsManager> PhysicsManager::s_instance{};
	PhysicsManager::PhysicsManager()
	{
	}
	PhysicsManager::~PhysicsManager()
	{
	}
	auto PhysicsManager::Initialize() -> void
	{
		if (s_instance == nullptr)
		{
			std::lock_guard<SpinLock> guard{ s_instanceChecker };
			if (s_instance == nullptr)
			{
				s_instance.reset(new PhysXPhysicsManager{});
			}
		}
	}
	auto PhysicsManager::Instance() -> std::shared_ptr<PhysicsManager>
	{
		return s_instance;
	}
	auto PhysicsManager::Release() -> void
	{
		std::lock_guard<SpinLock> guard{ s_instanceChecker };
		s_instance.reset();
	}

	Game::ComponentTag<PhysicsComponent> const PhysicsComponent::TAG{ "PhysicsComponent" };
	PhysicsComponent::PhysicsComponent() :
		Component{ TAG },
		physicsManager_{}
	{
		Bind(EVT_PHYSICS_POST_UPDATE, &PhysicsComponent::OnPhysicsPostUpdate);
	}
	PhysicsComponent::PhysicsComponent(PhysicsComponent&& rhs) noexcept :
		Component(rhs),
		physicsManager_{ std::move(rhs.physicsManager_) },
		offset_{ std::move(rhs.offset_) }
	{
	}
	PhysicsComponent::~PhysicsComponent()
	{

	}
	auto PhysicsComponent::Clone() const -> Game::Component*
	{
		return nullptr;
	}
	auto PhysicsComponent::OnPhysicsPostUpdate(Game::Event const& evt) -> void
	{
		auto object{ GetObj().lock() };
	}

}
using namespace physx;
