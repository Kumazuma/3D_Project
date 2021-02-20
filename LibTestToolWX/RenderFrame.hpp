#pragma once
#include<wx/wx.h>
#include<memory>
#include<GraphicsModule.hpp>
#include<SwapChain.hpp>
class RenderCanvas : public wxControl
{
public:
	RenderCanvas(wxWindow* parent, wxWindowID id);
	std::shared_ptr<Kumazuma::GraphicsModule> GetGraphicsMoudle();
	std::shared_ptr<Kumazuma::SwapChain> GetSwapChain();
private:
	std::shared_ptr<Kumazuma::GraphicsModule> graphicsModule_;
	std::shared_ptr<Kumazuma::SwapChain> swapChain_;
};