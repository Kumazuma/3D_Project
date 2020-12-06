#include"pch.h"
#include"include/game/object.hpp"
#include"include/game/runtime.hpp"
using namespace Kumazuma::Game;

Kumazuma::Game::Object::Object():
	m_id{0}
{

}

bool Kumazuma::Game::Object::HasTag(const ObjectTag& tag) const
{
	return m_tags.find(&tag) != m_tags.end();
}
Kumazuma::Game::Object::Object(Object&& other) noexcept:
	m_id{other.m_id},
	m_runtime{other.m_runtime},
	m_tags{std::move(other.m_tags)}
{
}
Component* Object::GetCom(const ComTagBase& tag) const
{
	auto coms = m_runtime->m_objectNComs.find(m_id);
	if (coms == m_runtime->m_objectNComs.end())
	{
		return nullptr;
	}
	for (auto& com : coms->second)
	{
		if (com->GetTag() == tag)
		{
			return &*com;
		}
	}
	return nullptr;
}