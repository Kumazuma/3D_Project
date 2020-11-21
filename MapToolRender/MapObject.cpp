#include "pch.h"
#include "MapToolRender.h"
namespace MapToolRender
{
	using namespace System;
	auto MapObject::Name::get()->String^
	{
		return m_name;
	}
	auto MapObject::Name::set(String^ value)->void
	{
		m_name = value;
		PropertyChanged(this, gcnew PropertyChangedEventArgs("Name"));
	}
}