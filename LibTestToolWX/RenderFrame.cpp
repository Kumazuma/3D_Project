#include "wx/wxprec.h"
#include "RenderFrame.hpp"
#include <OBJMesh.hpp>
RenderCanvas::RenderCanvas(wxWindow* parent, wxWindowID id):
	wxControl(parent, id)
{
	auto graphicsModule = Kumazuma::GraphicsModule::Create(GetHWND(), { 1920, 1080 }, false);
	graphicsModule_ = std::shared_ptr<Kumazuma::GraphicsModule>(graphicsModule);
}

std::shared_ptr<Kumazuma::GraphicsModule> RenderCanvas::GetGraphicsMoudle()
{
	return graphicsModule_;
}
