#pragma once
#include"transform.hpp"
#include"game.hpp"
#include <memory>
#include<set>
#include<list>
#include"comtag.hpp"
#include"runtime.hpp"
#include<unordered_set>
#include<unordered_map>
#include<shared_mutex>
namespace Kumazuma
{
	namespace Game
	{
		class Object final:public std::enable_shared_from_this<Object>
		{
			friend class Component;
			friend class Runtime;
			//cons & decons
		public:
			template<typename _ObjTagTIt>
			Object(_ObjTagTIt begin, _ObjTagTIt end);
			Object() = default;
			Object(const Object&) = delete;
			Object(Object&& other) noexcept;
			~Object() = default;
		public:
			template<typename _COM>
			auto GetComponent()->std::shared_ptr<_COM>;
			template<typename _COM>
			auto GetComponent()const->std::shared_ptr<const _COM>;
			template<typename _COM, typename ...Args>
			auto AddComponent(Args&& ...args)->void;
			template<typename _COM>
			auto RemoveComponent()->void;
		
		public:
			auto Tag(ObjectTag const& rTag)->void;
			auto HasTag(ObjectTag const&)const->bool;
		private:
			auto GetComponent(ComTagBase const* comtagPtr)const->std::shared_ptr<Component>;
			auto AddComponent(ComTagBase const* comtag, Component* ptr)->void;
			auto RemoveComponent(ComTagBase const* comtag)->void;
		private:
			std::unordered_map<ComTagBase const*, std::shared_ptr<Component> > m_components;
			std::unordered_set<ObjectTag const*> m_tags;
			mutable std::shared_mutex m_mutex;
		};
		template<typename _ObjTagTIt>
		inline Object::Object(_ObjTagTIt begin, _ObjTagTIt end):
			Object{}
		{
			for (; begin != end; ++begin)
			{
				m_tags.insert(*begin);
			}
		}
		template<typename _COM>
		inline auto Object::GetComponent()->std::shared_ptr<_COM>
		{
			auto ptr = GetComponent(&_COM::GetComponentTag());
			if (ptr == nullptr) return nullptr;
			return std::static_pointer_cast<_COM>(ptr);
		}
		template<typename _COM>
		inline auto Object::GetComponent() const->std::shared_ptr<const _COM>
		{
			auto ptr = GetComponent(&_COM::GetComponentTag());
			if (ptr == nullptr) return nullptr;
			return std::static_pointer_cast<const _COM>(ptr);
		}
		template<typename _COM, typename ...Args>
		inline auto Object::AddComponent(Args && ...args) -> void
		{
			AddComponent(&_COM::GetComponentTag(), new _COM{ std::forward<Args>(args)... });
		}
		template<typename _COM>
		inline auto Object::RemoveComponent() -> void
		{
			RemoveComponent(&_COM::GetComponentTag());
		}
		class ObjectTag
		{
		public:
			ObjectTag(const char* const lpszTagStr) :m_lpszTagStr{ lpszTagStr } {}
			ObjectTag(const ObjectTag&) = delete;
			ObjectTag(ObjectTag&&) = delete;
			bool operator == (const ObjectTag& other)const { return this == &other; }
			bool operator != (const ObjectTag& other)const { return this != &other; }
		private:
			const char* const m_lpszTagStr;
		};
	}
}