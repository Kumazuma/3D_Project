#pragma once
#include "common.hpp"
#include "Mesh.hpp"
namespace Kumazuma
{
	class SkinnedAnimationController;
	class DLL_CLASS SkinnedMesh : public Mesh
	{
	public:
		virtual void SetupIA(ID3D11DeviceContext* device) = 0;
		virtual Subsets const& GetSubsetsRef() = 0;
		virtual Texture2D* GetMaterialTexture(wchar_t const* materialName) = 0;
	};
}