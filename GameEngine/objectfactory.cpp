#include "pch.h"
#include "./include/game/objectfactory.hpp"
#include "./include/game/runtime.hpp"
using namespace Kumazuma::Game;
void ObjectFactory::AddComponent(IComponentFactory* com)
{
	auto& coms = m_components;
	coms[&com->GetTag()] = std::unique_ptr<IComponentFactory>(com);
}
Kumazuma::Game::ObjectFactory::ObjectFactory(ObjectFactory&& other) noexcept:
	m_tags{std::move(other.m_tags)},
	m_components{std::move(other.m_components)}
{
}
ObjectFactory& ObjectFactory::Tag(const ObjectTag& tag)&
{
	m_tags.insert(&tag);
	return *this;
}
ObjectFactory ObjectFactory::Tag(const ObjectTag& tag)&&
{
	m_tags.insert(&tag);
	return std::move(*this);
}
std::shared_ptr<Object> ObjectFactory::Create(Runtime& runtime) const
{
	return runtime.CreateObject(*this);
}

void ObjectFactory::Clear()
{
	m_components.clear();
	m_tags.clear();
}