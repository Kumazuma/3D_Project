#include "pch.h"
#include "include/RenderComponent.hpp"
#include <typedef.hpp>

IMPLEMENT_COMPONENT_CLASS(Kumazuma::Graphics::RenderComponent)
_BEGIN_NS(Kumazuma::Graphics)
RenderComponent::RenderComponent():
	Component{ TAG }
{

}

auto RenderComponent::PrepareRender(RenderModule& renderModule, GameRenderer& render) -> void
{
}

_END_NS
