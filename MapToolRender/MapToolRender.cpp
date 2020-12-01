#include "pch.h"

#include <d3d9.h>
#pragma unmanaged
#pragma warning(push)
#pragma warning(once: 4793)
#include<DirectXMath.h>
#pragma warning(pop)
#pragma managed
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "DxErr.lib")
#pragma comment(lib, "windowscodecs.lib")
#include "msclr\marshal_cppstd.h"
using namespace msclr::interop;
#include "MapToolRender.h"
#include <RenderModule/RenderModule.h>
#include "COMPtr.hpp"
#include "XMeshObj.h"
using namespace DirectX;
auto MapToolRender::GraphicsDevice::Initialize(System::Windows::Forms::Control^ renderView, unsigned width, unsigned height) -> void
{
	if (s_instance != nullptr)
		return;
	s_instance = gcnew MapToolRender::GraphicsDevice(renderView, width, height);
}
auto MapToolRender::GraphicsDevice::GetOpenFilePath(System::Windows::Forms::Control^ owner, System::String^ filter) -> System::String^
{
	OPENFILENAMEW ofn{};
	wchar_t filePath[1024]{};
	marshal_context ctx;
	std::wstring szfilter{ ctx.marshal_as<std::wstring>(filter) };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = nullptr;
	ofn.hInstance = nullptr;
	ofn.lpstrFilter = szfilter.c_str();
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = 1024;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_FILEMUSTEXIST;
	if (GetOpenFileNameW(&ofn) != 0)
	{
		System::String^ res{ ctx.marshal_as<System::String^>(filePath) };
		return res;
	}
	return nullptr;
	// TODO: 여기에 return 문을 삽입합니다.
}
auto MapToolRender::GraphicsDevice::GetSaveFilePath(System::Windows::Forms::Control^ owner, System::String^ filter) -> System::String^
{
	// TODO: 여기에 return 문을 삽입합니다.
	return nullptr;
}
auto MapToolRender::GraphicsDevice::Render(Control^ drawPanel, IEnumerable<RenderObject^>^ objs, Camera^ camera) -> void
{
	System::Threading::Monitor::Enter(this);
	m_pRenderModule->SetCamera(camera->PositionPtr, camera->RotationPtr);
	m_pRenderModule->SetProj(45.f, 1.f, 0.1f, 2000.f);
	try
	{
		if (!m_pRenderModule->Renderable())
		{
			return;
		}
		COMPtr<IDirect3DDevice9> pDevice;
		m_pRenderModule->GetDevice(&pDevice);

		ApplyViewProjMatrix();
		m_pRenderModule->PrepareFrustum();
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		for each (auto obj in objs)
		{
			auto* handle{ obj->Handle };
			if (handle != nullptr)
			{
				handle->PrepareRender(m_pRenderModule);
			}
		}
		m_pRenderModule->Render(0.f, 0.f, 1.f, 1.f, (HWND) drawPanel->Handle.ToPointer() );
	}
	finally
	{
		System::Threading::Monitor::Exit(this);
	}

}
auto MapToolRender::GraphicsDevice::ApplyViewProjMatrix() -> void
{
	
}

MapToolRender::GraphicsDevice::!GraphicsDevice()
{
	if (m_pRenderModule != nullptr)
	{
		delete m_pRenderModule;
		m_pRenderModule = nullptr;
	}
}

MapToolRender::GraphicsDevice::GraphicsDevice(Control^ renderView, unsigned width, unsigned height)
{
	void* hwnd{ renderView->Handle.ToPointer() };
	RenderModule* pOut{};
	RenderModule::Create((HWND)hwnd, width, height, &pOut);
	m_pRenderModule = pOut;
}

auto MapToolRender::MapObject::ToString() -> System::String^
{
	return String::Format("{0}({1})", m_name, GetType());
}

MapToolRender::MapObject::MapObject(MapObject^rhs):
	m_name{rhs->m_name}
{
}
