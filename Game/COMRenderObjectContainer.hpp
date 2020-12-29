#pragma once
#include<game/component.hpp>
#include<game/runtime.hpp>
#include<shared_mutex>
struct RenderObject;
namespace Kumazuma::Client
{
	class COMRenderObjectContainer: public Game::Component
	{
		using Self = COMRenderObjectContainer;
		using Collection = std::unordered_map<std::wstring, std::shared_ptr<RenderObject> >;
	public:
		static Game::ComponentTag<Self> TAG;
		COMRenderObjectContainer();
		COMRenderObjectContainer(Self const& rhs);
		auto Remove(std::wstring const& key)->void;
		auto Insert(std::wstring const& key, std::shared_ptr<RenderObject> obj)->void;
		auto Get(std::wstring const& key)->std::shared_ptr<RenderObject>;
		auto Clone()const->Game::Component* override;
		auto begin()const->Collection::const_iterator;
		auto end()const->Collection::const_iterator;
	private:
		Collection  m_collection;
		mutable std::shared_mutex m_mutex;
	};
}