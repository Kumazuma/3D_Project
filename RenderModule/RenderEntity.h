#pragma once
#include"Common.h"
class RenderModule;
struct DLL_CLASS RenderEntity
{
	virtual ~RenderEntity() = default;
	virtual auto Render(RenderModule*) -> void = 0;
};