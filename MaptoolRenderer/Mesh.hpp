#pragma once
class RenderObject;
class IRendererBase;
struct ID3DXEffect;
namespace MaptoolRenderer {
	public ref class Mesh
	{
	protected:
		Mesh();
	public:
		!Mesh();
		~Mesh();
		virtual auto Clone()->Mesh^ = 0;
	internal:
		auto Render(IRendererBase* renderer, ID3DXEffect* effect)->void;
	internal:
		RenderObject* renderObject_;
	protected:
	};
}
