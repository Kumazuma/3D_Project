#include "wx/wxprec.h"
#include "App.hpp"
#include "RenderFrame.hpp"
#include <Texture2D.hpp>
#include <OBJMesh.hpp>
#include <array>
wxWindowID const ID_CANVAS = 1001;
bool LibTestApp::OnInit()
{
    auto frame{ new wxFrame(nullptr, wxID_ANY, wxT("dx11")) };
    auto renderCanvas{ new RenderCanvas(frame, ID_CANVAS) };
    auto sizer{ new wxBoxSizer(wxVERTICAL) };
    sizer->Add(renderCanvas, 1, wxEXPAND);
    frame->SetSizer(sizer);
    frame->Layout();
    frame->Show();
    auto graphicsModule{ renderCanvas->GetGraphicsMoudle() };
    IDXGISwapChain* swapChain;
    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    ID3D11RenderTargetView* rtv;
    graphicsModule->GetSwapChain(&swapChain);
    graphicsModule->GetImmediateContext(&deviceContext);
    graphicsModule->GetDevice(&device);
    swapChain->Present(0, 0);
    graphicsModule->GetSwapChainTexture()->GetView(&rtv);
    deviceContext->ClearRenderTargetView(rtv, std::array<f32, 4>{0.f, 0.f, 1.f, 1.f}.data());
    auto* objMesh = Kumazuma::OBJMesh::Create(graphicsModule.get(), L"./chino/gctitm001.obj");
    OutputDebugStringA(typeid(*objMesh).name());
    delete objMesh;
    deviceContext->Release();
    swapChain->Release();
    rtv->Release();
    device->Release();

    return true;
}
wxIMPLEMENT_APP(LibTestApp);