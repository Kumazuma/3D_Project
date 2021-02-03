#pragma once
#include"common.hpp"
namespace Kumazuma
{
	namespace Game
	{
		class DLL_CLASS ComTagBase
		{
		public:
			ComTagBase(const char* const tagName) :m_tagName{ tagName } {}
			virtual ~ComTagBase() = default;
			bool operator == (const ComTagBase& other)const;
			bool operator != (const ComTagBase& other)const;
		private:
			const char* const m_tagName;
		};
		inline bool ComTagBase::operator==(const ComTagBase& other)const
		{
			return this == &other;
		}
		inline bool ComTagBase::operator!=(const ComTagBase& other)const
		{
			return this == &other;
		}
		template<typename COM>
		class ComponentTag : public ComTagBase
		{
		public:
			ComponentTag(const char* const tagName) :ComTagBase{ tagName } {}
			auto COMType()const->COM const& { return *((COM const*)nullptr); }
		};
	}
}