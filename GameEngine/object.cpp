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
auto Kumazuma::Game::Object::Tag(ObjectTag const& rTag) -> void
{
	m_tags.emplace(&rTag);
}
bool Kumazuma::Game::Object::HasTag(const ObjectTag& tag) const
{
	return m_tags.find(&tag) != m_tags.end();
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
