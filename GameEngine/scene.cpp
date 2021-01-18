#include "pch.h"
#include "./include/game/scene.hpp"
#include "./include/game/runtime.hpp"
Kumazuma::Game::Scene::Scene(Scene&& other) noexcept:
	m_objects{std::move(other.m_objects)}
{
}
void Kumazuma::Game::Scene::AddObject(LayerTag const& id, std::shared_ptr<Object> ptr)
{
	std::lock_guard<std::shared_mutex> guard{ m_mutex };
	this->m_objects[&id].push_back(std::move(ptr));
}

void Kumazuma::Game::Scene::RemoveObject(LayerTag const& id)
{
	std::lock_guard<std::shared_mutex> guard{ m_mutex };
	this->m_objects.erase(&id);
}
void Kumazuma::Game::Scene::RemoveObject(LayerTag const& id, Object& obj)
{
	std::lock_guard<std::shared_mutex> guard{ m_mutex };
	auto& list{ m_objects[&id] };
	auto it{ list.begin() };
	auto const end{ list.end() };
	while (it != end)
	{
		if (it->get() == &obj)
		{
			it = list.erase(it);
			continue;
		}
		++it;
	}
}
void Kumazuma::Game::Scene::RemoveObject(Object& obj)
{
	std::lock_guard<std::shared_mutex> guard{ m_mutex };

	for (auto& pair : m_objects)
	{
		auto& list{ pair.second };
		auto it{ list.begin() };
		auto const end{ list.end() };
		while(it != end)
		{
			if (it->get() == &obj)
			{
				it = list.erase(it);
				continue;
			}
			++it;
		}
	}
}

auto Kumazuma::Game::Scene::GetListRef(LayerTag const& id)->ObjList const&
{
	return m_objects[&id];
}

