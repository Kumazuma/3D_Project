#pragma once
class RenderObject;
class IRendererBase;
struct ID3DXEffect;
using namespace System;
namespace MaptoolRenderer {
	ref class MeshObject;
	public ref class Mesh abstract
	{
	protected:
		Mesh();
	public:
		!Mesh();
		~Mesh();
		virtual auto Clone()->Mesh^ = 0;
		property String^ FilePath
		{
			auto get()->String^;
		}
	public:

	internal:
		virtual auto Render(IRendererBase* renderer, ID3DXEffect* effect)->void;
	internal:
		::RenderObject* renderObject_;
		System::String^ filePath_;

	protected:
	};
}
