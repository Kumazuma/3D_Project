#pragma once
#include"Mesh.hpp"
class SimpleBoxObject;
namespace MaptoolRenderer
{
	private ref class BoxMeshPtr
	{
	public:
		BoxMeshPtr();
		!BoxMeshPtr();
		~BoxMeshPtr();
	internal:
		SimpleBoxObject* ptr_;
		int count_;
	};
	public ref class BoxMesh : public Mesh
	{
	public:
		BoxMesh();
		~BoxMesh();
		!BoxMesh();
		virtual auto Clone()->Mesh^ override;

	private:
		static System::WeakReference<BoxMeshPtr^>^ s_ptr;
		BoxMeshPtr^ inner_;
	};
}