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
	public:
		static Game::ComponentTag<Self> TAG;
		COMRenderObjectContainer();
		COMRenderObjectContainer(Self const& rhs);
		auto Remove(std::wstring const& key)->void;
		auto Insert(std::wstring const& key, std::shared_ptr<RenderObject> obj)->void;
		auto Get(std::wstring const& key)->std::shared_ptr<RenderObject>;
		auto Clone()const->Game::Component* override;
	private:
		std::unordered_map<std::wstring, std::shared_ptr<RenderObject> > m_collection;
		mutable std::shared_mutex m_mutex;
	};
}