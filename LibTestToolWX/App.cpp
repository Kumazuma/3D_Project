#include "wx/wxprec.h"
#include "App.hpp"
#include "setup.h"
#include "RenderFrame.hpp"
#include <Texture2D.hpp>
#include <OBJMesh.hpp>
#include <array>
#include <RenderSystem.hpp>
#include <StandardMaterial.hpp>
#include <Subsets.hpp>



using namespace DirectX;
wxWindowID const ID_CANVAS = 1001;
bool LibTestApp::OnInit()
{
    auto frame{ new wxFrame(nullptr, wxID_ANY, wxT("dx11")) };
    renderCanvas_ = new RenderCanvas(frame, ID_CANVAS);
    auto sizer{ new wxBoxSizer(wxVERTICAL) };
    sizer->Add(renderCanvas_, 1, wxEXPAND);
    frame->SetSizer(sizer);
    frame->Layout();
    frame->ShowFullScreen(true,0);
    auto graphicsModule{ renderCanvas_->GetGraphicsMoudle() };
    XMFLOAT4X4 viewSpace{};
    XMFLOAT4X4 projSpace{};
    XMStoreFloat4x4(&viewSpace, XMMatrixLookAtLH({ 0.f, 12.f, -2.f, 0.f }, { 0.f, 12.f, 0.f, 0.f }, { 0.f, 1.f, 0.f, 0.f }));
    XMStoreFloat4x4(&projSpace, XMMatrixPerspectiveFovLH(XMConvertToRadians(45.f), 1920.f/1080.f, 0.1f, 1000.f));

    XMStoreFloat4x4(&worldSpace, XMMatrixScaling(12.f, 12.f, 1.f));
    mesh_ = std::shared_ptr<Kumazuma::OBJMesh>(Kumazuma::OBJMesh::Create(graphicsModule.get(), L"./chino/gctitm001.obj"));

    renderSystem_.reset(Kumazuma::RenderSystem::Create(graphicsModule.get(), renderCanvas_->GetSwapChain().get()));


    for (int i = 0; i < mesh_->GetSubsetsRef().GetCount(); ++i)
    {
        auto* standardMaterial = new Kumazuma::StandardMaterial(graphicsModule.get(), &mesh_->GetSubsetsRef().Get(i));
        renderSystem_->AddMaterial(standardMaterial);
        standardMaterial->SetWorldMatrixPtr(&worldSpace);
        materials_.push_back(std::unique_ptr<Kumazuma::Material>(standardMaterial));
    }
    renderSystem_->Render(graphicsModule.get(), &viewSpace, &projSpace);

    renderCanvas_->GetSwapChain()->Present();

    //this->Bind(wxEVT_IDLE, &LibTestApp::OnIdle, this);
    frame->Bind(wxEVT_CLOSE_WINDOW, &LibTestApp::OnClose, this);

    timer_ = new wxTimer(this);
    this->Bind(wxEVT_TIMER, &LibTestApp::OnTimer, this);
    timer_->Start(1);
    return true;
}
int LibTestApp::OnExit()
{
    timer_->Stop();
    delete timer_;
    return 0;
}
void LibTestApp::OnIdle(wxIdleEvent& evt)
{
}
void LibTestApp::OnClose(wxCloseEvent& evt)
{
    evt.Skip();
    auto graphicsModule{ renderCanvas_->GetGraphicsMoudle() };
    for (auto& material : materials_)
    {
        renderSystem_->RemoveMaterial(material.get());
    }
    renderCanvas_ = nullptr;
}
void LibTestApp::OnTimer(wxTimerEvent& evt)
{
    if (renderCanvas_ == nullptr)return;
    XMFLOAT4X4 viewSpace{};
    XMFLOAT4X4 projSpace{};
    auto size = renderCanvas_->GetClientSize();
    XMStoreFloat4x4(&viewSpace, XMMatrixLookAtLH({ 0.f, 12.f, -2.f, 0.f }, { 0.f, 12.f, 0.f, 0.f }, { 0.f, 1.f, 0.f, 0.f }));
    XMStoreFloat4x4(&projSpace, XMMatrixPerspectiveFovLH(XMConvertToRadians(45.f), 1920.f / 1080.f, 0.1f, 1000.f));

    auto graphicsModule{ renderCanvas_->GetGraphicsMoudle() };
    renderSystem_->Render(graphicsModule.get(), &viewSpace, &projSpace);
    renderCanvas_->GetSwapChain()->Present();

}
wxIMPLEMENT_APP(LibTestApp);