#pragma once
#include"Common.h"
#include"RenderModule.h"
#include<d3d9.h>
class RenderModule;
struct IRenderer;
struct IDirect3DIndexBuffer9;
struct DLL_CLASS RenderEntity
{
	virtual ~RenderEntity() = default;
	virtual auto Render(RenderModule*, IRenderer*) -> void = 0;
	virtual auto SetRenderKind(RenderModule::Kind kind)->void {};
	virtual auto GetRenderKind()const->RenderModule::Kind {return RenderModule::Kind::NONE;}
};