#include "pch.h"
#include "SkyBoxObject.h"
#include "RenderModule.h"
#include <DirectXMath.h>
using namespace DirectX;
auto SkyBoxObject::Create(RenderModule* pRenderModule, SkyBoxObject** pOut) -> HRESULT
{
	HRESULT hr{};
	if (pOut == nullptr)
	{
		return E_POINTER;
	}
	auto obj = new SkyBoxObject{};
	hr = obj->Initialize(pRenderModule);
	if (FAILED(hr))
	{
		delete obj;
		obj = nullptr;
	}
	*pOut = obj;
	return hr;
}

auto SkyBoxObject::Render(RenderModule* pRenderModule) -> void
{
	if (m_pTexture == nullptr)
	{
		return;
	}
	COMPtr<IDirect3DDevice9> pDevice;
	pRenderModule->GetDevice(&pDevice);
	D3DMATRIX viewMatrix;
	D3DMATRIX projMatrix;
	D3DMATRIX worldTransfromMaxtrix;
	XMFLOAT4X4& rWorldTransform{ reinterpret_cast<XMFLOAT4X4&>(worldTransfromMaxtrix) };
	XMMATRIX mViewSpace{};
	XMMATRIX mWorldTransfrom{XMMatrixIdentity()};
	XMMATRIX mProjSpace{};
	XMVECTOR vFar{ XMVectorSet(0.f, 0.f, 1.f, 0.f)};
	pDevice->GetTransform(D3DTS_VIEW, &viewMatrix);
	pDevice->GetTransform(D3DTS_PROJECTION, &projMatrix);
	
	mProjSpace = XMLoadFloat4x4(&reinterpret_cast<XMFLOAT4X4&>(projMatrix));
	mViewSpace = XMLoadFloat4x4(&reinterpret_cast<XMFLOAT4X4&>(viewMatrix));
	XMMATRIX mCameraTransform;
	mCameraTransform = XMMatrixInverse(nullptr, mViewSpace);
	vFar = XMVector3TransformCoord(vFar, XMMatrixInverse(nullptr, mProjSpace));
	float scaleFactor = XMVectorGetZ(vFar * 0.5f);
	mWorldTransfrom = XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor);
	mWorldTransfrom.r[3] = mCameraTransform.r[3];
	XMStoreFloat4x4(&rWorldTransform, mWorldTransfrom);
	
	pDevice->SetTransform(D3DTS_WORLD, &worldTransfromMaxtrix);

	pDevice->SetTexture(0, m_pTexture.Get());
	// 그래픽 디바이스 장치에게 현재 내가 그리려는 버퍼를 링크시켜주는 함수
	pDevice->SetStreamSource(0, m_pVertexBuffer.Get(), 0, VERTEX_SIZE);
	// 1인자 : 몇 번 슬롯에 보관할 것인가
	// 2인자 : 어떤 것을 넘겨줄 것인가
	// 3인자 : 어디서부터 그릴 것인가
	// 4인자 : 어떤 단위로 표현할 것인가

	pDevice->SetFVF(FVF);
	pDevice->SetIndices(m_pIndexBuffer.Get());
	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
	DWORD lighting{};
	DWORD zWriteEnable{};
	pDevice->GetRenderState(D3DRS_LIGHTING, &lighting);
	pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &zWriteEnable);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vertexCount, 0, m_indexCount);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, zWriteEnable);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_LIGHTING, lighting);
}

auto SkyBoxObject::Clone() const -> RenderObject*
{
    return nullptr;
}

auto SkyBoxObject::SetDiffuseTexture(IDirect3DCubeTexture9* pTexture) -> void
{
    m_pTexture = MakeCOMPtr(std::move(pTexture));
}

auto SkyBoxObject::Initialize(RenderModule* pRenderModule) -> HRESULT
{
	COMPtr<IDirect3DDevice9> pDevice;
	m_vertexCount = 8;
	m_indexCount = 6 * 2;
	pRenderModule->GetDevice(&pDevice);
	HRESULT hr{ E_FAIL };
	UINT length{ VERTEX_SIZE * m_vertexCount };
	hr =
		pDevice->CreateVertexBuffer(
			length,
			0,
			FVF,
			D3DPOOL_DEFAULT,
			&m_pVertexBuffer,
			nullptr
		);
	hr =
		pDevice->CreateIndexBuffer(
			INDEX_SIZE * m_indexCount,
			0,
			INDEX_TYPE,
			D3DPOOL_DEFAULT,
			&m_pIndexBuffer,
			nullptr
		);
	VERTEX<FVF>* pVertices{ nullptr };
	Index<INDEX_TYPE>* pIndices{ nullptr };
	m_pVertexBuffer->Lock(
		0,
		0,
		&reinterpret_cast<void*&>(pVertices),
		0
	);
	m_pIndexBuffer->Lock(
		0,
		0,
		&reinterpret_cast<void*&>(pIndices),
		0
	);
	// 전면
	pVertices[0].vPosition = { -1.f, 1.f, -1.f };
	pVertices[1].vPosition = {1.f, 1.f, -1.f };
	pVertices[2].vPosition = {1.f, -1.f, -1.f };
	pVertices[3].vPosition = {-1.f, -1.f, -1.f };
	pVertices[0].vUV = pVertices[0].vPosition;
	pVertices[1].vUV = pVertices[1].vPosition;
	pVertices[2].vUV = pVertices[2].vPosition;
	pVertices[3].vUV = pVertices[3].vPosition;

	// 후면
	pVertices[4].vPosition = { -1.f, 1.f, 1.f };
	pVertices[5].vPosition = {1.f, 1.f, 1.f };
	pVertices[6].vPosition = {1.f, -1.f, 1.f };
	pVertices[7].vPosition = {-1.f, -1.f, 1.f };
	pVertices[4].vUV = pVertices[4].vPosition;
	pVertices[5].vUV = pVertices[5].vPosition;
	pVertices[6].vUV = pVertices[6].vPosition;
	pVertices[7].vUV = pVertices[7].vPosition;


	// x+
	pIndices[0][0] = 1;
	pIndices[0][1] = 5;
	pIndices[0][2] = 6;

	pIndices[1][0] = 1;
	pIndices[1][1] = 6;
	pIndices[1][2] = 2;

	// x-
	pIndices[2][0] = 4;
	pIndices[2][1] = 0;
	pIndices[2][2] = 3;

	pIndices[3][0] = 4;
	pIndices[3][1] = 3;
	pIndices[3][2] = 7;

	// y+
	pIndices[4][0] = 4;
	pIndices[4][1] = 5;
	pIndices[4][2] = 1;

	pIndices[5][0] = 4;
	pIndices[5][1] = 1;
	pIndices[5][2] = 0;

	// y-
	pIndices[6][0] = 3;
	pIndices[6][1] = 2;
	pIndices[6][2] = 6;

	pIndices[7][0] = 3;
	pIndices[7][1] = 6;
	pIndices[7][2] = 7;

	// z+
	pIndices[8][0] = 7;
	pIndices[8][1] = 6;
	pIndices[8][2] = 5;

	pIndices[9][0] = 7;
	pIndices[9][1] = 5;
	pIndices[9][2] = 4;

	// z-
	pIndices[10][0] = 0;
	pIndices[10][1] = 1;
	pIndices[10][2] = 2;

	pIndices[11][0] = 0;
	pIndices[11][1] = 2;
	pIndices[11][2] = 3;
	m_pIndexBuffer->Unlock();
	m_pVertexBuffer->Unlock();
	
    return S_OK;
}

SkyBoxObject::SkyBoxObject():
	m_vertexCount{},
	m_indexCount{},
	m_width{},
	m_depth{},
	m_interval{},
	m_maxHeight{}
{

}

SkyBoxObject::SkyBoxObject(SkyBoxObject const* rhs):
	m_vertexCount	{ rhs->m_vertexCount },
	m_indexCount	{ rhs->m_indexCount },
	m_width			{ rhs->m_width },
	m_depth			{ rhs->m_depth },
	m_interval		{ rhs->m_interval },
	m_maxHeight		{ rhs->m_maxHeight },
	m_pVertexBuffer	{ rhs->m_pVertexBuffer },
	m_pIndexBuffer	{ rhs->m_pIndexBuffer },
	m_pTexture		{rhs->m_pTexture }
{
}
