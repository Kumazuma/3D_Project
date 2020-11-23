#pragma once
#include"RenderObject.h"
#include"COMPtr.hpp"
#include<vector>
#include<memory>
#include<string>
#include<d3dx9.h>
class DLL_CLASS XMeshObject : public RenderObject
{
public:
	auto SetDiffuseTexture(u32 index, IDirect3DTexture9* pTexture)->void;
	auto GetDiffuseTextureName(u32 index)->std::wstring;
protected:
	u32 m_stride;
	u32 m_vertexCount;
	u32 m_subsetCount;
	std::vector<D3DXMATERIAL> m_materials;
	std::vector<COMPtr<IDirect3DTexture9> > m_textures;
	//Copied플래그, Copy on write를 구현하기 위한 변수
	bool m_copied;

};

class DLL_CLASS StaticXMeshObject : public XMeshObject
{
public:
	static auto Create(RenderModule* pRenderModule, std::wstring const& filePath)->HRESULT;
	auto Render(RenderModule* pRenderModule)->void override;
	auto Clone()const->RenderObject*;

protected:
	auto Initialize(RenderModule* pRenderModule, u32 width, u32 height, f32 interval, f32 terrainMaxHeight, u8 const* pArray)->HRESULT;
private:

};