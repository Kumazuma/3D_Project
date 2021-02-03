#pragma once
#include"Common.h"
#include<d3d9.h>
#include<d3dx9effect.h>
#include<DirectXMath.h>
class RenderModule;
struct IRenderer;
struct IRendererBase;
class Frustum;
class Ray;
namespace Kumazuma
{
	namespace Graphics
	{

		enum class RenderObjectKind {
			None,
			Mesh,
			MeshEffect,
			Particle,
			Sprite,
		};
		class DLL_CLASS RenderObjectV2
		{
		public:
			virtual ~RenderObjectV2() = default;
			virtual auto Clone(RenderObjectV2** pout)const->void = 0;
			virtual auto GetKind()const->RenderObjectKind = 0;
		};
	}
}

class DLL_CLASS RenderObject
{
protected:
	RenderObject();
public:
	virtual ~RenderObject() = default;
	//virtual auto PrepareRender(IRenderer* pDevice)->void = 0;
	virtual auto Clone()const->RenderObject* = 0;
	virtual auto CanRayPicking()const->bool;
	virtual auto RayPicking(DirectX::XMFLOAT3 const& rayAt, DirectX::XMFLOAT3 const& rayDirection, f32* pOut)->bool;
	virtual auto Render(IRendererBase* renderer, ID3DXEffect* effect)->void = 0;
	auto RayPicking(Ray* pRay, f32* pOut)->bool;
	//auto SetTransform(DirectX::XMFLOAT4X4 const& transform)->void;
	//auto GetTransform()const->DirectX::XMFLOAT4X4 const&;
	auto SetVisible(bool visible)->void;
	auto IsVisible()const->bool;
protected:
	bool m_visible;
	//DirectX::XMFLOAT4X4 m_transform;
public:
	static constexpr unsigned long FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	static constexpr unsigned long FVF_NAVI = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	static constexpr unsigned long FVF_TEX3D = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	template<unsigned long FVFTYPE>
	struct VERTEX;
	template<>
	struct VERTEX<FVF_TEX>
	{
		DirectX::XMFLOAT3 vPosition;
		DirectX::XMFLOAT3 vNormal;
		DirectX::XMFLOAT2 vUV;
	};
	template<>
	struct VERTEX<FVF_TEX3D>
	{
		DirectX::XMFLOAT3 vPosition;
		DirectX::XMFLOAT3 vUV;
	};
	template<>
	struct VERTEX<FVF_NAVI>
	{
		DirectX::XMFLOAT3 vPosition;
		DWORD diffuseColor;
	};

	template<unsigned long INDEXTYPE>
	struct MetaIndexData{};
	template<>
	struct MetaIndexData<D3DFMT_INDEX16>
	{
		using IndexType = u16;
	};
	template<>
	struct MetaIndexData<D3DFMT_INDEX32>
	{
		using IndexType = u32;
	};
	template<unsigned long INDEXTYPE>
	struct Index
	{
		using IndexType = typename MetaIndexData<INDEXTYPE>::IndexType;
		IndexType var[3];
		template<typename _INDEX>
		IndexType& operator[](_INDEX&& i) { return var[i]; }
		template<typename _INDEX>
		IndexType const& operator[](_INDEX&& i)const { return var[i]; }
	};
};
