#include "pch.h"
#include "./include/game/scene.hpp"
#include "./include/game/runtime.hpp"
Kumazuma::Game::Scene::Scene(Scene&& other) noexcept:
	m_objects{std::move(other.m_objects)}
{
}
void Kumazuma::Game::Scene::AddObject(std::shared_ptr<Object> ptr)
{
	m_objects.push_back(ptr);
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
