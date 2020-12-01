#pragma once
#include "MapToolRender.h"
#include "TextureCollection.h"
namespace MapToolRender
{
	public ref class WowMapMesh : public RenderObject
	{
	public:
		WowMapMesh(GraphicsDevice^ device, System::String^ filePath);
		auto Clone()->RenderObject^ override;
		property System::String^ MeshFilePath
		{
			auto get()->System::String^;
		}
		property Position^ Center
		{
			auto get()->Position^;
		}
	protected:
		WowMapMesh(WowMapMesh^ const& rhs);
	private:
		System::String^ m_filePath;
	};
}