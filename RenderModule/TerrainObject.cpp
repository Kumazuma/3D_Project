#include "pch.h"
#include "TerrainObject.h"
#include "RenderModule.h"
#pragma comment(lib, "d3d9.lib")
#undef max
using namespace DirectX;
auto TerrainObject::Initialize(RenderModule* pRenderModule, u32 const width, u32 const imgHeight, f32 const interval, f32 const terrainMaxHeight, u8 const* const pArray) -> HRESULT
{
	COMPtr<IDirect3DDevice9Ex> pDevice{};
	m_width = width;
	m_depth = imgHeight;
	m_maxHeight = terrainMaxHeight;
	pRenderModule->GetDevice(&pDevice);
	HRESULT hr{ E_FAIL };
	DWORD fvf = FVF;
    m_indexCount = (m_width - 1) * (m_depth - 1) * 2;
    m_vertexCount = m_width * m_depth;
	m_pVertexPositions.reset(new std::vector<XMFLOAT3A>());
	std::vector<XMFLOAT3A>& rVertexPosition{ *m_pVertexPositions };
	rVertexPosition.resize(m_vertexCount, XMFLOAT3A{ 0.f, 0.f, 0.f });
	m_interval = interval;
	UINT length{ VERTEX_SIZE * m_vertexCount };
	hr =
		pDevice->CreateVertexBuffer(
			length,
			0,
			fvf,
			D3DPOOL_DEFAULT,
			&m_pVertexBuffer,
			nullptr
		); 
	switch (hr)
	{
	case D3DERR_INVALIDCALL:
		"D3DERR_INVALIDCALL"; __debugbreak();
		break;
	case D3DERR_OUTOFVIDEOMEMORY:
		"D3DERR_OUTOFVIDEOMEMORY"; __debugbreak();

		break;
	case E_OUTOFMEMORY:
		"E_OUTOFMEMORY"; __debugbreak();
		break;
	}
	if (FAILED(hr))
		return hr;
	hr =
		pDevice->CreateIndexBuffer(
			INDEX_SIZE * m_indexCount,
			0,
			INDEX_TYPE,
			D3DPOOL_DEFAULT,
			&m_pIndexBuffer,
			nullptr
		);
	assert(SUCCEEDED(hr));
	if (FAILED(hr))
		return hr;
	VERTEX<FVF_TEX>* pVertices = nullptr;
	hr = m_pVertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	if (FAILED(hr))
		return hr;
	constexpr f32 U8MAX{ std::numeric_limits<u8>::max()  };
	for (u32 i = 0; i < m_depth; ++i)
	{
		f32 const z{ static_cast<f32>(i) };
		f32 const u{ z / static_cast<f32>(m_depth - 1) };
		for (u32 j = 0; j < m_width; ++j)
		{
			f32 const x{ static_cast<f32>(j) };
			f32 const v{ static_cast<f32>(j) / static_cast<f32>(m_width - 1) };
			u32 index = i * m_width + j;
			XMFLOAT3A postion{};
			postion.x = x;
			postion.z = z;
			postion.y =terrainMaxHeight * static_cast<f32>(pArray[index * 4]) / U8MAX;
			pVertices[index].vPosition = postion;
			pVertices[index].vUV = {u,v};
			pVertices[index].vNormal = {};
			rVertexPosition[index] = postion;
		}
	}
	Index<INDEX_TYPE>* pIndices{ nullptr };
	m_pIndexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
	u32 lastIdx{  };
	for (u32 i = 0; i < m_depth - 1; ++i)
	{
		for (u32 j = 0; j < m_width - 1; ++j)
		{
			for (u32 k = 0; k < 2; ++k)
			{
				u32 idx{ lastIdx++ };
				u32 vertexIdx{ i * m_width + j };

				pIndices[idx][0] = vertexIdx + m_width;
				pIndices[idx][1] = vertexIdx + (!k?1:0) * m_width + 1;
				pIndices[idx][2] = vertexIdx +  (!k ? 1 : 0);

				XMVECTOR vRight{};
				XMVECTOR vUp{};
				auto& originVertex{ pVertices[pIndices[idx][0]] };
				auto& rightVertex{ pVertices[pIndices[idx][1]] };
				auto& upVertex{ pVertices[pIndices[idx][2]] };
				XMVECTOR vOrigin{ XMLoadFloat3(&originVertex.vPosition) };
				
				vRight = XMLoadFloat3(&rightVertex.vPosition);
				vRight -= vOrigin;
				
				vUp = XMLoadFloat3A(rVertexPosition.data() + pIndices[idx][2]);
				vUp -= vOrigin;

				XMVECTOR vNormal{ XMVector3Cross(vRight, vUp) };

				XMVECTOR vOriginNormal{ XMLoadFloat3(&originVertex.vNormal) };
				XMVECTOR vRightNormal{ XMLoadFloat3(&rightVertex.vNormal) };
				XMVECTOR vUpNormal{XMLoadFloat3(&upVertex.vNormal)};
				vOriginNormal += vNormal;
				vRightNormal += vNormal;
				vUpNormal += vNormal;
				XMStoreFloat3(&originVertex.vNormal, vOriginNormal);
				XMStoreFloat3(&rightVertex.vNormal, vRightNormal);
				XMStoreFloat3(&upVertex.vNormal, vUpNormal);
			}
		}
	}
	const auto pEnd{ pVertices + m_vertexCount };
	
	for (auto it = pVertices; it != pEnd; ++it)
	{
		XMVECTOR vNormal{ XMLoadFloat3( &it->vNormal) };
		XMStoreFloat3(&it->vNormal, XMVector3Normalize(vNormal));
	}

	m_pVertexBuffer->Unlock();
	m_pIndexBuffer->Unlock();
    return S_OK;
}
TerrainObject::TerrainObject():
    m_fvf{ FVF_TEX },
    m_indexCount{},
    m_vertexCount{},
	m_interval{ },
	m_copied{ false }
{

}



TerrainObject::TerrainObject(TerrainObject const* rhs):
	m_fvf{ rhs->m_fvf },
	m_pVertexPositions{rhs->m_pVertexPositions},
	m_indexCount{ rhs->m_indexCount},
	m_pIndexBuffer {rhs->m_pIndexBuffer},
	m_pVertexBuffer{rhs->m_pVertexBuffer},
	m_vertexCount{rhs->m_vertexCount},
	m_interval{rhs->m_interval},
	m_copied{true}
{

}

auto TerrainObject::Render(RenderModule* pRenderModule) -> void
{
	COMPtr<IDirect3DDevice9Ex> pDevice{};
	pRenderModule->GetDevice(&pDevice);
	if (m_pTexture == nullptr)
	{
		COMPtr<IDirect3DTexture9> pDefaultTexture;
		pRenderModule->GetDefaultTexture(&pDefaultTexture);
		pDevice->SetTexture(0, pDefaultTexture.Get());
	}
	else
	{
		pDevice->SetTexture(0, m_pTexture.Get());
	}
	XMMATRIX mWorldMatrix{ XMLoadFloat4x4(&m_transform) };
	XMVECTOR vPos{ mWorldMatrix.r[3] };
	mWorldMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	mWorldMatrix = XMMatrixScaling(m_interval, 1.f, m_interval) * mWorldMatrix;
	mWorldMatrix.r[3] = vPos;
	D3DMATRIX worldMat{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&worldMat), mWorldMatrix);
    pDevice->SetFVF(FVF);
    pDevice->SetTransform(D3DTS_WORLD,&worldMat);
    pDevice->SetStreamSource(0, m_pVertexBuffer.Get(), 0, VERTEX_SIZE);
    pDevice->SetIndices(m_pIndexBuffer.Get());
    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vertexCount, 0, m_indexCount);
}

auto TerrainObject::Clone() const -> RenderObject*
{
	return new TerrainObject{ this };
}

auto TerrainObject::SetDiffuseTexture(IDirect3DTexture9* pTexture) -> void
{
    m_pTexture = MakeCOMPtr(std::move(pTexture));
}


auto TerrainObject::Create(RenderModule* pRenderModule, u32 width, u32 height, f32 interval, f32 terrainMaxHeight, u8 const* pArray, TerrainObject** pObj) -> HRESULT
{
    TerrainObject* pTerrainObject{};
    pTerrainObject = new TerrainObject{};
    HRESULT hr{};
    hr = pTerrainObject->Initialize(pRenderModule, width, height, interval, terrainMaxHeight, pArray);
    if (FAILED(hr))
    {
        delete pTerrainObject;
        pTerrainObject = nullptr;
        return hr;
    }
    *pObj = pTerrainObject;
	return S_OK;
}
auto TerrainObject::SetInterval(f32 value)->void
{
	m_interval = value;
}
auto TerrainObject::GetInterval()const->f32
{
	return m_interval;
}
auto TerrainObject::SetMaxHeight(f32 value)->void
{
	if (m_copied)
	{
		//Max Height가 변경되면 이전 버텍스 버퍼와는 정보가 달라지므로 카피해야 한다.
		HRESULT hr{};
		COMPtr<IDirect3DDevice9> pDevice;
		m_pVertexBuffer->GetDevice(&pDevice);
		std::shared_ptr<std::vector<DirectX::XMFLOAT3A> > newVertexPosition{ new std::vector<XMFLOAT3A>{} };
		*newVertexPosition = *m_pVertexPositions;
		m_pVertexPositions = std::move(newVertexPosition);
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
		m_copied = false;
	}

	HRESULT hr{};
	f32 newMaxHeight{ value };
	VERTEX<FVF_TEX>* pVertices = nullptr;
	hr = m_pVertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	if (FAILED(hr))
		return ;
	constexpr f32 U8MAX{ std::numeric_limits<u8>::max() };
	std::vector<XMFLOAT3A>& rVertexPosition{ *m_pVertexPositions };
	rVertexPosition.resize(m_vertexCount, XMFLOAT3A{ 0.f, 0.f, 0.f });

	for (u32 i = 0; i < m_depth; ++i)
	{
		f32 const z{ static_cast<f32>(i) };
		f32 const u{ z / static_cast<f32>(m_depth - 1) };
		for (u32 j = 0; j < m_width; ++j)
		{
			f32 const x{ static_cast<f32>(j) };
			f32 const v{ static_cast<f32>(j) / static_cast<f32>(m_width - 1) };
			u32 index = i * m_width + j;
			XMFLOAT3A postion{};
			rVertexPosition[index].y = newMaxHeight * rVertexPosition[index].y / m_maxHeight;
			postion.x = x;
			postion.z = z;
			postion.y = rVertexPosition[index].y;
			pVertices[index].vPosition = postion;
			pVertices[index].vUV = { u,v };
			pVertices[index].vNormal = {};
		}
	}
	u32 lastIdx{  };
	for (u32 i = 0; i < m_depth - 1; ++i)
	{
		for (u32 j = 0; j < m_width - 1; ++j)
		{
			for (u32 k = 0; k < 2; ++k)
			{
				u32 idx{ lastIdx++ };
				u32 vertexIdx{ i * m_width + j };
				Index<INDEX_TYPE> index;
				index[0] = vertexIdx + m_width;
				index[1] = vertexIdx + (!k ? 1 : 0) * m_width + 1;
				index[2] = vertexIdx + (!k ? 1 : 0);

				XMVECTOR vRight{};
				XMVECTOR vUp{};
				auto& originVertex{ pVertices[index[0]] };
				auto& rightVertex{ pVertices[index[1]] };
				auto& upVertex{ pVertices[index[2]] };
				XMVECTOR vOrigin{ XMLoadFloat3(&originVertex.vPosition) };

				vRight = XMLoadFloat3(&rightVertex.vPosition);
				vRight -= vOrigin;

				vUp = XMLoadFloat3A(rVertexPosition.data() + index[2]);
				vUp -= vOrigin;

				XMVECTOR vNormal{ XMVector3Cross(vRight, vUp) };

				XMVECTOR vOriginNormal{ XMLoadFloat3(&originVertex.vNormal) };
				XMVECTOR vRightNormal{ XMLoadFloat3(&rightVertex.vNormal) };
				XMVECTOR vUpNormal{ XMLoadFloat3(&upVertex.vNormal) };
				vOriginNormal += vNormal;
				vRightNormal += vNormal;
				vUpNormal += vNormal;
				XMStoreFloat3(&originVertex.vNormal, vOriginNormal);
				XMStoreFloat3(&rightVertex.vNormal, vRightNormal);
				XMStoreFloat3(&upVertex.vNormal, vUpNormal);
			}
		}
	}
	const auto pEnd{ pVertices + m_vertexCount };

	for (auto it = pVertices; it != pEnd; ++it)
	{
		XMVECTOR vNormal{ XMLoadFloat3(&it->vNormal) };
		XMStoreFloat3(&it->vNormal, XMVector3Normalize(vNormal));
	}
	m_pVertexBuffer->Unlock();
	m_maxHeight = value;
}
auto TerrainObject::GetMaxHeight()const->f32
{
	return m_maxHeight;
}