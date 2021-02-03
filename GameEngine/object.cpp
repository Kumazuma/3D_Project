#include"pch.h"
#include"include/game/object.hpp"
#include"include/game/runtime.hpp"
using namespace Kumazuma::Game;



Kumazuma::Game::Object::Object(Object&& other) noexcept:
	m_components{std::move(other.m_components)},
	m_mutex{},
	m_tags{std::move(other.m_tags)}
{

}
Kumazuma::Game::Object::~Object()
{
	std::lock_guard<decltype(m_mutex)> otherGuard{ m_mutex };
	auto parent{ m_parent.lock() };
	if (parent != nullptr)
	{
		parent->NotifyDeleteChild(*this);
	}
}
auto Kumazuma::Game::Object::Tag(ObjectTag const& rTag) -> void
{
	m_tags.emplace(&rTag);
}
bool Kumazuma::Game::Object::HasTag(const ObjectTag& tag) const
{
	return m_tags.find(&tag) != m_tags.end();
}
auto Kumazuma::Game::Object::RemoveChild(Game::Object& child) -> void
{
	std::lock_guard<std::shared_mutex> guard{ m_mutex };
	auto it = std::find_if(m_children.begin(), m_children.end(), [&child](auto const& it)->bool {return &child == it.second.get(); });
	if (it != m_children.end())
	{
		child.m_parent = std::weak_ptr<Game::Object>{};
		m_children.erase(it);
	}
}
auto Kumazuma::Game::Object::RemoveChild(std::wstring const& id) -> void
{
	std::lock_guard<std::shared_mutex> guard{ m_mutex };
	auto it{ m_children.find(id) };
	if (it != m_children.end())
	{
		it->second->m_parent = std::weak_ptr<Game::Object>{};
		m_children.erase(it);
	}
}
auto Kumazuma::Game::Object::GetChild(std::wstring const& id) -> std::shared_ptr<Object>
{
	std::shared_lock<std::shared_mutex> guard{ m_mutex };
	auto it{ m_children.find(id) };
	if (it != m_children.end())
	{
		return it->second;
	}
	return std::shared_ptr<Object>();
}
auto Kumazuma::Game::Object::GetChildren(std::wstring const& id) -> std::shared_ptr<Object>
{
	std::shared_lock<std::shared_mutex> guard{ m_mutex };
	auto it{ m_children.find(id) };
	if (it != m_children.end())
	{
		return it->second;
	}

	return std::shared_ptr<Object>();
}
auto Kumazuma::Game::Object::AddChild(std::wstring const& id, std::shared_ptr <Object> const& child) -> void
{
	if (child.get() == this) return;
	std::lock_guard<std::shared_mutex> guard{ child->m_mutex };
	auto ptr{ child->m_parent.lock() };
	if (ptr != nullptr)return;
	std::lock_guard<std::shared_mutex> guard2{ m_mutex };
	m_children.emplace(id, child);
	child->m_parent = this->weak_from_this();
}
auto Kumazuma::Game::Object::GetChildren() ->std::unordered_map<std::wstring, std::shared_ptr <Object> > const&
{
	return m_children;
}
auto Kumazuma::Game::Object::GetParent() const -> std::shared_ptr<Object const>
{
	return m_parent.lock();
}
auto Kumazuma::Game::Object::NotifyDeleteChild(Game::Object& child) -> void
{
	std::lock_guard<std::shared_mutex> guard{ m_mutex };
	auto it = std::find_if(m_children.begin(), m_children.end(), [&child](auto const& it)->bool {return &child == it.second.get(); });
	if (it != m_children.end())
	{
		m_children.erase(it);
	}
}
auto Kumazuma::Game::Object::GetComponent(ComTagBase const* comtagPtr) const -> std::shared_ptr<Component>
{
	std::shared_lock<std::shared_mutex> guard{ m_mutex };
	const auto it = m_components.find(comtagPtr);
	if (it == m_components.end()) return nullptr;
	return it->second;
}

auto Kumazuma::Game::Object::AddComponent(ComTagBase const* comtag, Component* ptr) -> void
{
	std::lock_guard<std::shared_mutex> guard{ m_mutex };
	ptr->SetObject(this->weak_from_this());
	m_components.emplace(comtag, Runtime::Instance()->AddComponent(comtag, ptr));
}

auto Kumazuma::Game::Object::RemoveComponent(ComTagBase const* comtag) -> void
{
	std::lock_guard<std::shared_mutex> guard{ m_mutex };
	m_components.erase(comtag);
}
