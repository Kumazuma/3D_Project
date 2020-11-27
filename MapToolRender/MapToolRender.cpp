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
auto MapToolRender::GraphicsDevice::Render() -> void
{
	System::Threading::Monitor::Enter(this);
	try
	{
		if (!m_pRenderModule->Renderable())
		{
			return;
		}
		COMPtr<IDirect3DDevice9> pDevice;
		m_pRenderModule->GetDevice(&pDevice);

		ApplyViewProjMatrix();
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pRenderModule->BeginRender(0.f, 0.f, 1.f, 1.f);

		auto list = m_renderObjects[RenderGroup::PRIORITY];
		for each (auto obj in list)
		{
			auto* handle{ obj->Handle };
			if (handle != nullptr)
			{
				handle->Render(m_pRenderModule);
			}
		}
		list = m_renderObjects[RenderGroup::NONALPHA];
		for each (auto obj in list)
		{
			auto* handle{ obj->Handle };
			if (handle != nullptr)
			{
				handle->Render(m_pRenderModule);
			}
		}
		list = m_renderObjects[RenderGroup::ALPHA];
		for each (auto obj in list)
		{
			auto* handle{ obj->Handle };
			if (handle != nullptr)
			{
				handle->Render(m_pRenderModule);
			}
		}
		m_pRenderModule->EndRender();
	}
	finally
	{
		System::Threading::Monitor::Exit(this);
	}
}

auto MapToolRender::GraphicsDevice::Render(Control^ renderView, RenderObject^ obj, Camera^ camera)->void
{
	System::Threading::Monitor::Enter(this);
	try
	{
		
		COMPtr<IDirect3DDevice9> pDevice;
		if (!m_pRenderModule->Renderable())
		{
			return;
		}
		m_pRenderModule->GetDevice(&pDevice);
		m_pRenderModule->SetCamera(camera->PositionPtr, camera->RotationPtr);
		m_pRenderModule->SetProj(45.f, 1.f, 0.1f, 2000.f);
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pRenderModule->BeginRender(0.f, 0.f, 1.f, 1.f);
		obj->Handle->Render(m_pRenderModule);
		m_pRenderModule->EndRender((HWND)renderView->Handle.ToPointer());
	}
	finally
	{
		System::Threading::Monitor::Exit(this);
	}
	
}

auto MapToolRender::GraphicsDevice::ClearRenderGroup() -> void
{
	m_renderObjects.Clear();
}

auto MapToolRender::GraphicsDevice::Add(RenderGroup groupId, RenderObject^ obj) -> void
{
	if (!m_renderObjects.ContainsKey(groupId))
	{
		m_renderObjects.Add(groupId, gcnew HashSet<RenderObject^>());
	}
	m_renderObjects[groupId]->Add(obj);
}

auto MapToolRender::GraphicsDevice::Remove(RenderGroup groupId, RenderObject^ obj) -> void
{
	if (m_renderObjects.ContainsKey(groupId))
	{
		m_renderObjects[groupId]->Remove(obj);
	}

}
auto MapToolRender::GraphicsDevice::CreateStaticMesh() -> StaticXMeshObj^
{

	wchar_t filePath[1024]{};
	OPENFILENAME ofn{};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = nullptr;
	ofn.hInstance = nullptr;
	ofn.lpstrFilter = L"X파일\0*.x\0";
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = 1024;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_FILEMUSTEXIST;
	if (GetOpenFileNameW(&ofn) != 0)
	{
		marshal_context ctx;
		System::String^ res{ ctx.marshal_as<System::String^>(filePath) };
		return gcnew StaticXMeshObj(this, res);
	}
	return nullptr;
}
auto MapToolRender::GraphicsDevice::ApplyViewProjMatrix() -> void
{
	m_pRenderModule->SetCamera(m_currentCamera->PositionPtr, m_currentCamera->RotationPtr);
	m_pRenderModule->SetProj(45.f, 1.f, 0.1f, 2000.f);
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
	m_currentCamera = gcnew Camera();
	auto position = m_currentCamera->Position;
	auto rotation = m_currentCamera->Rotation;
	position->Y = 1.f;
	position->Z = -1.f;
	rotation->X = DirectX::XMConvertToRadians(10.f);
	m_currentCamera->Position = position;
	m_currentCamera->Rotation = rotation;
	
	auto enums = Enum::GetValues(RenderGroup::typeid);
	for each (auto it in enums)
	{
		auto key = static_cast<RenderGroup>(it);
		m_renderObjects.Add(key, gcnew HashSet<RenderObject^>{});
	}
}

auto MapToolRender::MapObject::ToString() -> System::String^
{
	return String::Format("{0}({1})", m_name, GetType());
}

MapToolRender::MapObject::MapObject(MapObject^rhs):
	m_name{rhs->m_name}
{
}
