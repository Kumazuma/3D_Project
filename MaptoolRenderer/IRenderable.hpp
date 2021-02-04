#pragma once
namespace MaptoolRenderer {
	ref class GraphicsDevice;
	public interface struct IRenderable {
		virtual auto PrepereRender(GraphicsDevice^ renderer)->void = 0;
	};
	private interface struct IRenderEntity {
		virtual auto Render(GraphicsDevice^ renderer)->void = 0;
	};
}