#pragma once
#include"Mesh.hpp"
class SimpleSphereObject;
namespace MaptoolRenderer
{
	private ref class SphereMeshPtr
	{
	public:
		SphereMeshPtr();
		!SphereMeshPtr();
		~SphereMeshPtr();
	internal:
		SimpleSphereObject* ptr_;
		int count_;
	};
	public ref class SphereMesh : public Mesh
	{
	public:
		SphereMesh();
		~SphereMesh();
		!SphereMesh();
	private:
		static System::WeakReference<SphereMeshPtr^>^ s_ptr;
		SphereMeshPtr^ inner_;
	};
}