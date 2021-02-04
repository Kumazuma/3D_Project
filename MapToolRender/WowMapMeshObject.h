#pragma once
#include "MapToolRender.h"
#include "TextureCollection.h"

namespace MapToolRender
{
	[TypeConverter(EnumConverter::typeid)]
	public enum class Usage{None, Foliage, Structure, Terrain};
	public ref class WowMapMesh : public RenderObject
	{
	public:
		WowMapMesh(GraphicsDevice^ device, System::String^ filePath);
		auto Clone()->RenderObject^ override;
		property System::String^ MeshFilePath
		{
			auto get()->System::String^;
		}
		property MapToolCore::Position Center
		{
			auto get()->MapToolCore::Position;
		}
		[TypeConverter(EnumConverter::typeid)]
		property MapToolRender::Usage Usage
		{
			auto get()->MapToolRender::Usage;
			auto set(MapToolRender::Usage value)->void;
		}
	protected:
		WowMapMesh(WowMapMesh^ const& rhs);
	private:
		System::String^ m_filePath;
		MapToolRender::Usage m_usage;
	};
}
