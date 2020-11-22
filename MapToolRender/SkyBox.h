#pragma once
#include "MapToolRender.h"
namespace MapToolRender
{
	public ref class SkyBox : public RenderObject
	{
	public:
		SkyBox(GraphicsDevice^ device);
		auto Clone()->RenderObject^ override;
		property MapToolRender::Texture^ SkyBoxTexture
		{
			auto get()->MapToolRender::Texture^;
			auto set(MapToolRender::Texture^ texture)->void;
		}
	protected:
		SkyBox(SkyBox^ const& rhs);
		Texture^ m_texture;
	};
}