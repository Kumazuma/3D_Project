#pragma once
#include"Mesh.hpp"
namespace MaptoolRenderer {
	ref class GraphicsDevice;
	using namespace System::ComponentModel;
	public ref class OBJMesh : public Mesh
	{
	public:
		OBJMesh(System::String^ path);
		OBJMesh(OBJMesh^ rhs);
		auto Clone()->Mesh^ override;
		[BrowsableAttribute(false)]
		property array<System::String^>^ SubsetNames
		{
			auto get()->array<System::String^>^;
		}
		property MapToolCore::Position Position
		{
			auto get()->MapToolCore::Position;
		}
	internal:
		::RenderObject* renderObject_;
		array<System::String^>^ subsetNames_;

	protected:
	};
}