#include "wx/wxprec.h"
#include "RenderFrame.hpp"
#include <OBJMesh.hpp>
static std::weak_ptr<Kumazuma::GraphicsModule> g_graphicsModule;

RenderCanvas::RenderCanvas(wxWindow* parent, wxWindowID id):
	wxControl(parent, id)
{
	auto graphicsModule = g_graphicsModule.lock();
	if (graphicsModule == nullptr)
	{
		graphicsModule.reset(Kumazuma::GraphicsModule::Create( ));
		
	}
	graphicsModule_ = graphicsModule;
	swapChain_.reset(Kumazuma::SwapChain::Create(GetHWND(), graphicsModule.get(), { 1920, 1080 }, DXGI_FORMAT_R8G8B8A8_UNORM, false));

}

std::shared_ptr<Kumazuma::GraphicsModule> RenderCanvas::GetGraphicsMoudle()
{
	return graphicsModule_;
}

std::shared_ptr<Kumazuma::SwapChain> RenderCanvas::GetSwapChain()
{
	return swapChain_;
}
