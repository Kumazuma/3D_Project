#pragma once
#include"Common.h"
#include<d3d9.h>
#include<DirectXMath.h>
class RenderModule;
struct DLL_CLASS RenderObject
{
protected:
	RenderObject();
public:
	virtual ~RenderObject() = default;
	virtual auto Render(RenderModule* pDevice)->void = 0;
	virtual auto Clone()const->RenderObject* = 0;
	auto SetTransform(DirectX::XMFLOAT4X4 const& transform)->void;
	auto GetTransform()const->DirectX::XMFLOAT4X4 const&;
protected:
	DirectX::XMFLOAT4X4 m_transform;
protected:
	static constexpr unsigned long FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	template<unsigned long FVFTYPE>
	struct VERTEX;
	template<>
	struct VERTEX<FVF_TEX>
	{
		DirectX::XMFLOAT3 vPosition;
		DirectX::XMFLOAT3 vNormal;
		DirectX::XMFLOAT2 vUV;
	};
	template<unsigned long INDEXTYPE>
	struct Index;
	template<>
	struct Index<D3DFMT_INDEX32>
	{
		u32 var[3];
		template<typename INDEX>
		u32& operator [](INDEX i) { return var[i]; }
		template<typename INDEX>
		u32 const& operator [](INDEX i)const { return var[i]; }
	};
	template<>
	struct Index<D3DFMT_INDEX16>
	{
		u16 var[3];
		template<typename INDEX>
		u16& operator [](INDEX i) { return var[i]; }
		template<typename INDEX>
		u16 const& operator [](INDEX i)const { return var[i]; }
	};
};
