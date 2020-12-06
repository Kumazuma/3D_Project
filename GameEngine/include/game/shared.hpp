#pragma once
#include "runtime.hpp"
#include<atomic>
namespace Kumazuma
{
	namespace Game
	{
		template<typename T>
		class Runtime::Shared
		{
			struct ShareInfo
			{
				T* ptr;
				std::atomic<size_t> strongCount;
				std::atomic<size_t> weakCount;
			};
		public:

		private:

		};
	}
}