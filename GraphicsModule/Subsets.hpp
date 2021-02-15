#pragma once
#include "common.hpp"
#include "typedef.hpp"
namespace Kumazuma
{
	class Subset;
	class DLL_CLASS Subsets
	{
	public:
		virtual ~Subsets();
		virtual u32 GetCount() = 0;
		virtual Subset const& Get(u32 index) = 0;
		
	};
}