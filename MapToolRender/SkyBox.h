#pragma once
#include "MapToolRender.h"
namespace MapToolRender
{
	ref class CubeTexture;
	public ref class SkyBox : public RenderObject
	{
	public:
		SkyBox(GraphicsDevice^ device);
		auto Clone()->RenderObject^ override;
		property MapToolRender::CubeTexture^ SkyBoxTexture
		{
			auto get()->MapToolRender::CubeTexture^;
			auto set(MapToolRender::CubeTexture^ texture)->void;
		}
	protected:
		SkyBox(SkyBox^ const& rhs);
		CubeTexture^ m_texture;
	};
}