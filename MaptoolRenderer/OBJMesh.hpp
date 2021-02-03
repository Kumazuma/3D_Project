#pragma once
#include"Mesh.hpp"
namespace MaptoolRenderer {
	ref class GraphicsDevice;
	public ref class OBJMesh : public Mesh
	{
	public:
		OBJMesh(System::String^ path);
		OBJMesh(OBJMesh^ rhs);
		auto Clone()->Mesh^ override;
		property array<System::String^>^ SubsetNames
		{
			auto get()->array<System::String^>^;
		}

	internal:
		RenderObject* renderObject_;
	protected:
	};
}