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
#include "ControlSwapChain.h"
#include "COMPtr.hpp"
#include "XMeshObj.h"
#include "Ray.h"
#include <RenderModule/MapToolRenderer.h>
using namespace DirectX;
auto MapToolRender::GraphicsDevice::Initialize(System::Windows::Forms::Control^ renderView, unsigned width, unsigned height) -> void
{
	if (s_instance != nullptr)
		return;
	s_instance = gcnew MapToolRender::GraphicsDevice(renderView, width, height);
}
MapToolRender::GraphicsDevice::GraphicsDevice(Control^ renderView, unsigned width, unsigned height)
{
	void* hwnd{ renderView->Handle.ToPointer() };
	RenderModule* pOut{};
	MapToolRenderer* mapToolRenderer;
	RenderModule::Create((HWND)hwnd, width, height, &pOut);
	m_pRenderModule = pOut;
	HRESULT hr = MapToolRenderer::Create(m_pRenderModule, width, height, &mapToolRenderer);
	if (FAILED(hr)) throw gcnew System::Exception("can not create renderer!");
	m_pRenderer = mapToolRenderer;
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
auto MapToolRender::GraphicsDevice::Render(System::Windows::Forms::Control^ control, IEnumerable<RenderObject^>^ objs, Camera^ camera) -> void
{
	System::Threading::Monitor::Enter(this);
	DirectX::XMFLOAT4X4 projMatrix{};
	DirectX::XMFLOAT4X4 viewMatrix{};
	auto width{ m_pRenderModule->GetWidth() };
	auto height{ m_pRenderModule->GetHeight() };
	m_pRenderModule->GenerateProjPerspective(45.f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 2000.f, &projMatrix);
	m_pRenderModule->GenerateViewMatrix(*camera->PositionPtr, *camera->RotationPtr, &viewMatrix);
	m_pRenderer->SetProjMatrix(projMatrix);
	m_pRenderer->SetViewMatrix(viewMatrix);
	COMPtr<IDirect3DDevice9> pDevice;
	m_pRenderModule->GetDevice(&pDevice);
	try
	{
		if (!m_pRenderModule->Renderable())
		{
			
			HRESULT hr = pDevice->TestCooperativeLevel();
			if (hr == D3DERR_DEVICENOTRESET)
			{
				delete m_pRenderer;
				if (m_pRenderModule->Renderable())
				{
					MapToolRenderer* mapToolRenderer;
					
					hr = MapToolRenderer::Create(
						m_pRenderModule,
						m_pRenderModule->GetWidth(),
						m_pRenderModule->GetHeight(),
						&mapToolRenderer);
					if (FAILED(hr))throw gcnew System::Exception("could not create renderer!");
					m_pRenderer = mapToolRenderer;
				}
				else
				{
					return;
				}
			}
		}
		COMPtr<IDirect3DDevice9> pDevice;
		m_pRenderModule->GetDevice(&pDevice);

		ApplyViewProjMatrix();
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		for each (auto obj in objs)
		{
			auto* handle{ obj->Handle };
			if (handle != nullptr)
			{
				handle->PrepareRender(m_pRenderer);
			}
		}
		m_pRenderer->Render(m_pRenderModule);
		pDevice->Present(nullptr, nullptr, (HWND)control->Handle.ToPointer(), nullptr);
	}
	finally
	{
		System::Threading::Monitor::Exit(this);
	}

}
auto MapToolRender::GraphicsDevice::CreateMouseRay(Control^ drawPanel, Camera^ camera, System::Drawing::Point^ mousePt) -> Ray^
{
	auto clientSize{ drawPanel->ClientSize };
	float x{ static_cast<float>(mousePt->X) };
	float y{ static_cast<float>(mousePt->Y) };
	
	float width{ static_cast<float>(clientSize.Width) };
	float height{ static_cast<float>(clientSize.Height) };
	x = x / width;// 0~1f;
	x = x * 2.f;//0~2.f;
	x = x - 1.f; // -1 ~ 1
	y = -1.f * (2.f * y / height - 1.f);

	DirectX::XMFLOAT3 pos = m_pRenderModule->ConvertProjToWorld(
		*camera->PositionPtr,
		*camera->RotationPtr,
		45.f,
		1.f,
		0.1f,
		2000.f,
		{x, y, 0.f}
		);
	return gcnew Ray{ camera->PositionPtr, &pos };
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
	if (m_pRenderer != nullptr)
	{
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}


auto MapToolRender::MapObject::ToString() -> System::String^
{
	return String::Format("{0}({1})", m_name, GetType());
}

auto MapToolRender::MapObject::BroadcastPropertyChanged(System::String^ propertyName) -> void
{
	PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs(propertyName));
}

MapToolRender::MapObject::MapObject(MapObject^rhs):
	m_name{rhs->m_name}
{
}
