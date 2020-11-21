#include "pch.h"
#include <d3d9.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "windowscodecs.lib")

#include "MapToolRender.h"
#include <RenderModule/RenderModule.h>
#include "COMPtr.hpp"
using namespace DirectX;
auto MapToolRender::GraphicsDevice::Initialize(System::Windows::Forms::Control^ renderView, unsigned width, unsigned height) -> void
{
	if (s_instance != nullptr)
		return;
	s_instance = gcnew MapToolRender::GraphicsDevice(renderView, width, height);
}
auto MapToolRender::GraphicsDevice::Render() -> void
{
	COMPtr<IDirect3DDevice9Ex> pDevice;
	m_pRenderModule->GetDevice(&pDevice);

	ApplyViewProjMatrix();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 255, 255), 1.f, 0);
	pDevice->BeginScene();
	auto enums = Enum::GetValues(RenderGroup::typeid);
	for each (auto it in enums)
	{
		auto key = static_cast<RenderGroup>(it);
		if (m_renderObjects.ContainsKey(key))
		{
			auto  list = m_renderObjects[key];
			for each (auto obj in list)
			{
				auto* handle{ obj->Handle };
				if (handle != nullptr)
				{
					handle->Render(m_pRenderModule);
				}
				//obj->Handle->Render(m_pRenderModule);
			}
		}
	}
	//for each (auto var in m_renderObjects[RenderGroup::PRIORITY])
	//{
	//	TODO: 각 오브젝트의 렌더를 진행해야 한다.
	//} 
	pDevice->EndScene();
	pDevice->PresentEx(nullptr, nullptr, NULL, nullptr,0);
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
	
	
}
