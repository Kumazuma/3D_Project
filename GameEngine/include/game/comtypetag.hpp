#pragma once
namespace Kumazuma
{
	class ComponentTagBase
	{
	public:
		virtual ~ComponentTagBase() = default;
		bool operator==(const ComponentTagBase&) const;
		bool operator!=(const ComponentTagBase&) const;
	};
	inline bool ComponentTagBase::operator==(const ComponentTagBase& other ) const
	{
		return this == &other;
	}
	inline bool ComponentTagBase::operator!=(const ComponentTagBase& other) const
	{
		return this == &other;
	}

	template<typename ComDrivedType>
	class ComponentTag:public ComponentTagBase
	{

	};
}