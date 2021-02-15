#pragma once
#include<wx/wx.h>
class LibTestApp : public wxApp
{
public:
	virtual bool OnInit() override;
};
wxDECLARE_APP(LibTestApp);