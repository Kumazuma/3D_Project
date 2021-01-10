#pragma once
#include<game/component.hpp>
#include<game/runtime.hpp>
#include<shared_mutex>
#include<SpinLock.hpp>
#include<StringLiteral.h>
struct RenderObject;
namespace Kumazuma::Client
{
	class COMRenderObjectContainer: public Game::Component
	{
		using Self = COMRenderObjectContainer;
		using Collection = std::unordered_map<StringLiteral<wchar_t> , std::shared_ptr<RenderObject> >;
	public:
		static Game::ComponentTag<Self> TAG;
		COMRenderObjectContainer();
		COMRenderObjectContainer(Self const& rhs);
		auto Remove(StringLiteral<wchar_t> const& key)->void;
		auto Insert(StringLiteral<wchar_t> const& key, std::shared_ptr<RenderObject> obj)->void;
		auto Get(StringLiteral<wchar_t> const& key)->std::shared_ptr<RenderObject>;
		auto Clone()const->Game::Component* override;
		auto begin()const->Collection::const_iterator;
		auto end()const->Collection::const_iterator;
	private:
		Collection  m_collection;
		mutable SpinLock m_mutex;
	};
}