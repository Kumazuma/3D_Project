#pragma once
#include<wx/wx.h>
#include<memory>
#include<GraphicsModule.hpp>
class RenderCanvas : public wxControl
{
public:
	RenderCanvas(wxWindow* parent, wxWindowID id);
	std::shared_ptr<Kumazuma::GraphicsModule> GetGraphicsMoudle();
private:
	std::shared_ptr<Kumazuma::GraphicsModule> graphicsModule_;
};