#include "pch.h"
#include "RenderSystem.hpp"
#include "RenderSystemImpl.hpp"
namespace Kumazuma
{
	RenderSystem* RenderSystem::Create(GraphicsModule* gModule, SwapChain* swapChain)
	{
		return new RenderSystemImpl(gModule, swapChain);
	}
	RenderSystem::~RenderSystem()
	{
	}
}
