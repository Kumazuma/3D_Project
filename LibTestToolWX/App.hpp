#pragma once
#include<wx/wx.h>
#include<memory>
#include<OBJMesh.hpp>
#include<Material.hpp>
#include"RenderFrame.hpp"
class LibTestApp : public wxApp
{
public:
	virtual bool OnInit() override;
	virtual int OnExit() override;
protected:
	void OnIdle(wxIdleEvent& evt);
	void OnClose(wxCloseEvent& evt);
	void OnTimer(wxTimerEvent& evt);
	std::shared_ptr<Kumazuma::OBJMesh> mesh_;
	std::vector < std::unique_ptr<Kumazuma::Material> > materials_;
	DirectX::XMFLOAT4X4 worldSpace;
	wxTimer* timer_;
	RenderCanvas* renderCanvas_;
};
wxDECLARE_APP(LibTestApp);