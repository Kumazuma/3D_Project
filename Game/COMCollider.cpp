#include "COMCollider.hpp"
#include <mutex>
_BEGIN_NS(Kumazuma::Client)
IMPLEMENT_COMPONENT_CLASS(Kumazuma::Client::COMCollider)

COMCollider::COMCollider():
    Component(TAG)
{
}

Kumazuma::Client::COMCollider::COMCollider(COMCollider const& rhs):
    Component(rhs),
    colliders_{rhs.colliders_}
{
}

COMCollider::~COMCollider()
{
}

auto COMCollider::Clone() const -> Game::Component*
{
    std::lock_guard<decltype(lock_)> guard{ lock_ };
    return new COMCollider{ *this };
}

auto COMCollider::Add(std::wstring const& name, Collider const& collider) -> void
{
    std::lock_guard<decltype(lock_)> guard{ lock_ };
    colliders_.emplace(name, collider);
}

auto COMCollider::Remove(std::wstring const& name) -> void
{
    std::lock_guard<decltype(lock_)> guard{ lock_ };
    colliders_.erase(name);
}

auto COMCollider::Get(std::wstring const& name) -> std::optional<Collider>
{
    std::shared_lock<decltype(lock_)> guard{ lock_ };
    if (auto it = colliders_.find(name); it != colliders_.end())
    {
        return it->second;
    }
    return std::optional<Collider>();
}
auto Kumazuma::Client::COMCollider::GetColliderTableRef() const -> std::unordered_map<std::wstring, Collider> const&
{
    return this->colliders_;
}
_END_NS