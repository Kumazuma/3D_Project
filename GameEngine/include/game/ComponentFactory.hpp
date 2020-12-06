#pragma once
#include"component.hpp"
#include "comtag.hpp"
namespace Kumazuma
{
	namespace Game
	{
		class Component;
		struct IComponentFactory
		{
			virtual Component* Create()const = 0;
			virtual const ComTagBase& GetTag()const = 0;
			virtual ~IComponentFactory() = default;
		};
		template<typename ComT>
		class ComponentFactory : public IComponentFactory
		{
		public:
			virtual Component* Create()const override
			{
				return new ComT{};
			}
			virtual const ComTagBase& GetTag()const override
			{
				return ComT::TAG;
			}
		};
	}
}