#pragma once
#include"game.hpp"
#include<memory>
namespace Kumazuma
{
	namespace Game
	{
		class Module
		{
		public:
			virtual ~Module() = default;
			virtual void PostUpdate(Runtime& runtime) =0;
		protected:
		};
	}
}