#include "pch.h"
#include "Texture.h"
#include "MapToolRender.h"
#include <RenderModule/RenderModule.h>
#include "msclr\marshal_cppstd.h"
using namespace msclr::interop;
using namespace MapToolCore;
MapToolRender::Texture::Texture(GraphicsDevice^ device,  System::String^ filePath)
{
	marshal_context ctx;
	IDirect3DTexture9* pTexture{};
	
	wchar_t const* const szFilePath{ ctx.marshal_as<wchar_t const*>(filePath) };
	HRESULT hr{};
	hr = device->Handle->CreateTexture(szFilePath, &pTexture);
	assert(SUCCEEDED(hr));
	m_pTexture = pTexture;


	System::String^ projectDirectory{ MapToolCore::Environment::Instance->ProjectDirectory };
	System::String^ currentDirectory{ System::IO::Path::GetFullPath(projectDirectory) };
	System::String^ fileFullPath{ System::IO::Path::GetFullPath(filePath) };
	System::String^ path{ MapToolCore::Utility::GetRelativePath(currentDirectory, fileFullPath) };
	m_filePath = path;
	D3DSURFACE_DESC surfaceDesc{};
	m_pTexture->GetLevelDesc(0, &surfaceDesc);
	m_width = surfaceDesc.Width;
	m_height = surfaceDesc.Height;
}

MapToolRender::Texture::Texture():
	m_pTexture{ nullptr },
	m_filePath{}
{
	
}

MapToolRender::Texture::~Texture()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

MapToolRender::Texture::!Texture()
{
	this->~Texture();
}

MapToolRender::CubeTexture::CubeTexture(GraphicsDevice^ device, System::String^ filePath)
{
	marshal_context ctx;
	IDirect3DCubeTexture9* pTexture{};

	wchar_t const* const szFilePath{ ctx.marshal_as<wchar_t const*>(filePath) };
	HRESULT hr{};
	hr = device->Handle->CreateCubeTexture(szFilePath, &pTexture);
	assert(SUCCEEDED(hr));
	m_pTexture = pTexture;

	System::String^ projectDirectory{ MapToolCore::Environment::Instance->ProjectDirectory };
	System::String^ currentDirectory{ System::IO::Path::GetFullPath(projectDirectory) };
	System::String^ fileFullPath{ System::IO::Path::GetFullPath(filePath) };
	System::String^ path{ MapToolCore::Utility::GetRelativePath(currentDirectory, fileFullPath) };
	m_filePath = path;
	D3DSURFACE_DESC surfaceDesc{};
	m_pTexture->GetLevelDesc(0, &surfaceDesc);
	m_width = surfaceDesc.Width;
	m_height = surfaceDesc.Height;
}

MapToolRender::CubeTexture::CubeTexture()
{
}

MapToolRender::CubeTexture::~CubeTexture()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

MapToolRender::CubeTexture::!CubeTexture()
{
	this->~CubeTexture();

}
