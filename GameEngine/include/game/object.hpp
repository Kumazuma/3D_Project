#pragma once
#include"transform.hpp"
#include"game.hpp"
#include <memory>
#include<set>
#include<list>
#include"comtag.hpp"
#include"runtime.hpp"
#include<unordered_set>
namespace Kumazuma
{
	namespace Game
	{
		class Object
		{
			friend class Component;
			friend class Runtime;
		public:
			virtual ~Object() = default;
			bool IsValid()const { return m_valid; }

			size_t GetID() const { return m_id; }
			bool HasTag(const ObjectTag& tag)const;
			Object(const Object&) = delete;
			Object(Object&& other) noexcept;
			template<typename _ComT>
			std::shared_ptr<_ComT> GetComponent(const ComponentTag<_ComT>& tag);
			
			template<typename _ComT>
			std::shared_ptr<const _ComT> GetComponent(const ComponentTag<_ComT>& tag)const ;
			template<typename _ComT>
			const _ComT& GetComponentRef(const ComponentTag<_ComT>& tag)const;
		protected:
			Component* GetCom(const ComTagBase& tag) const;
			Object();
			template<typename _ObjTagTIt>
			Object(_ObjTagTIt begin, _ObjTagTIt end);
		private:
			bool m_valid;
			std::shared_ptr<const Runtime> m_runtime;
			std::unordered_set<const ObjectTag*> m_tags;
			size_t m_id;
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
		template<typename _ComT>
		inline std::shared_ptr<_ComT> Object::GetComponent(const ComponentTag<_ComT>& tag)
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
					return (std::shared_ptr< _ComT>&)com;
				}
			}
			return nullptr;
		}
		template<typename _ComT>
		inline std::shared_ptr<const _ComT> Object::GetComponent(const ComponentTag<_ComT>& tag) const
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
					return (std::shared_ptr<const _ComT>&)com;
				}
			}
			return nullptr;
		}
		template<typename _ComT>
		inline const _ComT& Object::GetComponentRef(const ComponentTag<_ComT>& tag)const
		{
			auto com = GetCom(tag);
			if (com != nullptr)
			{
				return (const _ComT&)*com;
			}
			return *(const _ComT*)nullptr;
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