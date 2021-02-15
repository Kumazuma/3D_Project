#include "pch.h"
#include "Material.hpp"
namespace Kumazuma
{
	Material::Material(MaterialShadingClass shadingClass):
		shadingClass_{shadingClass}
	{
	}
	Material::~Material()
	{
	}
	MaterialShadingClass Material::GetShadingClass() const
	{
		return shadingClass_;
	}
}

