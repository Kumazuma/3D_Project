#include "pch.h"
#include <RenderModule\RenderModule.h>
#include "CubeTexture.hpp"
#include "MaptoolRenderer.h"
#include "msclr\marshal_cppstd.h"
using namespace msclr::interop;
MaptoolRenderer::CubeTexture::CubeTexture(GraphicsDevice^ device, System::String^ filePath)
{
    marshal_context ctx;
    IDirect3DCubeTexture9* cubeTexture;
    std::wstring szFilePath{ ctx.marshal_as<std::wstring>(filePath) };
    device->RenderModuleHandle->CreateCubeTexture(szFilePath.c_str(), &cubeTexture);
    if (cubeTexture == nullptr)
    {
        throw gcnew System::Exception(String::Format("Failed Load Cube Texture({0})", filePath));
    }
    texture_ = cubeTexture;
    filePath_ = System::IO::Path::GetFullPath(filePath);
    D3DSURFACE_DESC surfaceDesc{};
    texture_->GetLevelDesc(0, &surfaceDesc);
    width_ = surfaceDesc.Width;
    height_ = surfaceDesc.Height;
}

MaptoolRenderer::CubeTexture::CubeTexture()
{

}

MaptoolRenderer::CubeTexture::~CubeTexture()
{
    if (texture_ != nullptr)
    {
        texture_->Release();
        texture_ = nullptr;
    }
}

MaptoolRenderer::CubeTexture::!CubeTexture()
{
    this->~CubeTexture();
}
