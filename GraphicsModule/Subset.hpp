#pragma once
#include "common.hpp"
#include "typedef.hpp"
#include <d3d11.h>
#include <string>
namespace Kumazuma
{
	class Mesh;
	class DLL_CLASS Subset
	{
	public:
		virtual ~Subset();
		virtual u32 GetTriangleCount()const = 0;
		virtual u32 GetIndexBase()const = 0;
		virtual Mesh& GetMesh()const = 0;
		virtual HRESULT GetVertexBuffer(ID3D11Buffer** out)const = 0;
		virtual HRESULT GetIndexBuffer(ID3D11Buffer** out)const = 0;
		virtual std::wstring const& GetMaterial()const = 0;
		virtual std::wstring const& GetName()const = 0;
	};
}