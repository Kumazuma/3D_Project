#include "ColliderManager.hpp"
#include "COMCollider.hpp"
#include <mutex>
_BEGIN_NS(Kumazuma::Client)
SpinLock ColliderManager::s_spinlock{};
std::shared_ptr<ColliderManager> ColliderManager::s_instance{};
auto ColliderManager::Initialize() -> bool
{
    if (s_instance == nullptr)
    {
        std::lock_guard<decltype(s_spinlock)> guard{ s_spinlock };
        if (s_instance == nullptr)
        {
            s_instance = std::make_shared<ColliderManager>();
            return true;
        }
    }
    return false;
}

auto ColliderManager::Release() -> void
{
    if (s_instance != nullptr)
    {
        std::lock_guard<decltype(s_spinlock)> guard{ s_spinlock };
        if (s_instance != nullptr)
        {
            auto s = s_instance;
            s_instance = nullptr;
        }
    }
}

auto ColliderManager::Instance() -> std::shared_ptr<ColliderManager>
{
    return s_instance;
}

auto Kumazuma::Client::ColliderManager::Register(COMCollider* component) -> void
{
    std::lock_guard<decltype(s_spinlock)> guard{ s_spinlock };
    this->colliders_.emplace(component);
}

auto Kumazuma::Client::ColliderManager::Unregister(COMCollider* component) -> void
{
    std::lock_guard<decltype(s_spinlock)> guard{ s_spinlock };
    this->colliders_.erase(component);

}

auto ColliderManager::Update(f32 timeDelta) -> void
{
    std::lock_guard<decltype(s_spinlock)> guard{ s_spinlock };
    auto it{ colliders_.begin() };
    auto const end{ colliders_.end() };
    
    for (; it != end; ++it)
    {
        
        auto it2{ it };
        ++it2;
        for (; it2 != end; ++it2)
        {

        }
    }
}

_END_NS