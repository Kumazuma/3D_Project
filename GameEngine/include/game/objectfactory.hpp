#pragma once
#include<memory>
#include<list>
#include"game.hpp"
#include<set>
#include "ComponentFactory.hpp"
#include<unordered_set>
namespace Kumazuma
{
	namespace Game
	{
		class Component;
		class Runtime;

		class ObjectFactory
		{
			friend class Runtime;
		public:
			template<typename _ComT>
			ObjectFactory& Component(ComponentFactory<_ComT>&& com)&;
			template<typename _ComT, typename ...Args>
			ObjectFactory& Component(Args&&... args)&;
			template<typename _ComT, typename ...Args>
			ObjectFactory Component(Args&&... args)&&;
			template<typename _ComT>
			ObjectFactory Component(ComponentFactory<_ComT>&& com)&&;
			ObjectFactory(const ObjectFactory& other) = delete;
			ObjectFactory(ObjectFactory&& other) noexcept;
			ObjectFactory() {}
			ObjectFactory& Tag(const Game::ObjectTag& tag)&;
			ObjectFactory Tag(const Game::ObjectTag& tag)&&;
			template<typename _ComT>
			ComponentFactory<_ComT>& GetFactory(const Game::ComponentTag<_ComT>& tag)
			{
				return static_cast<ComponentFactory<_ComT>&>(*m_components[&tag]);
			}
			void Clear();
		protected:
			void AddComponent(IComponentFactory* com);
		private:
			std::unordered_set<const Game::ObjectTag*> m_tags;
			std::unordered_map<const Game::ComTagBase* , std::unique_ptr<IComponentFactory>> m_components;
		};
		
		template<typename _ComT>
		inline ObjectFactory& ObjectFactory::Component(ComponentFactory<_ComT>&& com)&
		{
			static_assert(std::is_base_of<Kumazuma::Game::Component, _ComT>::value == true, "");
			AddComponent(new ComponentFactory<_ComT>{ std::move(com) });
			return *this;
		}
		
		template<typename _ComT, typename ...Args>
		inline ObjectFactory& ObjectFactory::Component(Args&&... args)&
		{
			static_assert(std::is_base_of<Kumazuma::Game::Component, _ComT>::value == true, "");
			AddComponent(new ComponentFactory<_ComT>{ std::forward<Args>(args)... });
			return *this;
		}
		template<typename _ComT>
		inline ObjectFactory ObjectFactory::Component(ComponentFactory<_ComT>&& com)&&
		{
			static_assert(std::is_base_of<Kumazuma::Game::Component, _ComT>::value == true, "");
			AddComponent(new ComponentFactory<_ComT>{ std::move(com) });
			return std::move(*this);
		}

		template<typename _ComT, typename ...Args>
		inline ObjectFactory ObjectFactory::Component(Args&&... args)&&
		{
			static_assert(std::is_base_of<Kumazuma::Game::Component, _ComT>::value == true, "");
			AddComponent(new ComponentFactory<_ComT>{ std::forward<Args>(args)... });
			return std::move(*this);
		}
	}
}