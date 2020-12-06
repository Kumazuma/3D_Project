#include "pch.h"
#include "./include/game/scene.hpp"
#include "./include/game/runtime.hpp"
Kumazuma::Game::Scene::Scene(Scene&& other) noexcept:
	m_objects{std::move(other.m_objects)}
{
}
std::shared_ptr<Kumazuma::Game::Object> Kumazuma::Game::Scene::NewObject(const ObjectFactory& factory)
{
	//Runtime::Instance()
	auto obj = factory.Create(*Runtime::Instance());
	m_objects.push_back(obj);
	return obj;
}

void Kumazuma::Game::Scene::RemoveObject(Object& obj)
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (it->get() == &obj)
		{
			m_objects.erase(it);
			break;
		}
	}
}
