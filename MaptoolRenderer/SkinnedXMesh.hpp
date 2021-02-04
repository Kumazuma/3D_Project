#pragma once
#include"Mesh.hpp"
namespace DirectX
{
	struct XMFLOAT4X4;
}
namespace MaptoolRenderer
{
	using namespace System;
	using namespace System::ComponentModel;
	public ref class SkinnedXMesh : public Mesh
	{
	public:
		SkinnedXMesh(String^ path);
		SkinnedXMesh(SkinnedXMesh^ rhs);
		[ReadOnlyAttribute(true)]
		[BrowsableAttribute(false)]
		property array<System::String^>^ FrameNames
		{
			auto get()->array<System::String^>^;
		}
		property System::UInt32 AnimationCount
		{
			auto get()->System::UInt32;
		}
		auto Clone()->Mesh^ override;
	internal:
		auto PlayAnimation(float timeDelta)->void;
		auto GetFrame(String^ frame, DirectX::XMFLOAT4X4* out)->void;
	private:
		array<System::String^>^ m_frameNames;
	};
}