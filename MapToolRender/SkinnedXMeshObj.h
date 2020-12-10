#pragma once
#include "MapToolRender.h"
#include "TextureCollection.h"
namespace MapToolRender
{
	using namespace System::Collections::Generic;
	ref class SkinnedXMeshFrameCollections;
	public ref class SkinnedXMeshObj : public RenderObject
	{
	public:
		SkinnedXMeshObj(GraphicsDevice^ device, System::String^ filePath);
		auto Clone()->RenderObject^ override;
		auto PlayAnimIdx(unsigned int idx)->void;
		virtual auto Update(int ms)->void override;
		property System::UInt32 AnimationCount
		{
			auto get()->System::UInt32;
		}
		property System::String^ MeshFilePath
		{
			auto get()->System::String^;
		}
		[BrowsableAttribute(false)]
		property SkinnedXMeshFrameCollections^ Frames
		{
			auto get()->SkinnedXMeshFrameCollections^;
		}

		[ReadOnlyAttribute(true)]
		[BrowsableAttribute(false)]
		property array<System::String^>^ FrameNames
		{
			auto get()->array<System::String^>^;
		}
	protected:
		SkinnedXMeshObj(SkinnedXMeshObj^ const& rhs);
	private:
		array<System::String^>^ m_frameNames;
		System::String^ m_filePath;
	};
	public ref class SkinnedXMeshFrameCollections
	{
	internal:
		SkinnedXMeshFrameCollections(::RenderObject* pMesh);
	public:
		property DirectX::XMFLOAT4X4 const* default[String ^ ]
		{
			auto get(String^ key)->DirectX::XMFLOAT4X4 const*;
		}
	private:
		::RenderObject* pMesh;
	};
}