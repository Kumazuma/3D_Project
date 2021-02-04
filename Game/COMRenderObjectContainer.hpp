#pragma once
#include<game/component.hpp>
#include<game/runtime.hpp>
#include<shared_mutex>
#include<SpinLock.hpp>
#include<StringLiteral.h>
#include<string>
class RenderObject;
//namespace Kumazuma::Client
//{
//	class COMRenderObjectContainer: public Game::Component
//	{
//		using Self = COMRenderObjectContainer;
//		using Collection = std::unordered_map<std::wstring, std::shared_ptr<MeshObject> >;
//	public:
//		
//		COMRenderObjectContainer();
//		COMRenderObjectContainer(Self const& rhs);
//		auto Remove(std::wstring const& key)->void;
//		auto Insert(std::wstring const& key, std::shared_ptr<MeshObject> obj)->void;
//		auto Get(std::wstring const& key)->std::shared_ptr<MeshObject>;
//		auto Clone()const->Game::Component* override;
//		auto begin()const->Collection::const_iterator;
//		auto end()const->Collection::const_iterator;
//	private:
//		Collection  m_collection;
//		mutable SpinLock m_mutex;
//		DECLARE_COMPONENT_CLASS(Kumazuma::Client::COMRenderObjectContainer)
//	};
//}