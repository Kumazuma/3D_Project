#pragma once
#pragma once
#include"RenderObject.h"
#include"COMPtr.hpp"
#include<vector>
#include<memory>
#include<string>
#include<d3dx9.h>
class DLL_CLASS XMeshObject : public RenderObject
{
protected:
	XMeshObject();
public:
	auto SetDiffuseTexture(u32 index, IDirect3DTexture9* pTexture)->void;
	auto GetDiffuseTextureName(u32 index)->std::wstring;
protected:
	u32 m_stride;
	u32 m_vertexCount;
	u32 m_subsetCount;
	bool m_copied;

	D3DXMATERIAL const* m_pMaterials;
	std::vector<COMPtr<IDirect3DTexture9> > m_textures;
	//Copied플래그, Copy on write를 구현하기 위한 변수

};
