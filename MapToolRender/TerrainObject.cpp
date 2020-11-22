#include"pch.h"
#include"MapToolRender.h"
#include "Texture.h"
#include<RenderModule/TerrainObject.h>
#include<RenderModule/RenderModule.h>

#include "msclr\marshal_cppstd.h"

using namespace msclr::interop;

MapToolRender::TerrainObject::TerrainObject(GraphicsDevice^ device, System::String^ filePath, float interval, float maxHeight)
{
	marshal_context ctx;
	::RenderObject* pTerrainObject;
	wchar_t const* const szFilePath{ ctx.marshal_as<wchar_t const*>(filePath) };
	HRESULT hr{};
	hr = device->Handle->CreateTerrain(szFilePath, 0, interval, maxHeight ,&pTerrainObject);
	if (FAILED(hr))
	{
		System::Exception^ ex =gcnew System::Exception("Failed Create Terrain Object");
		throw ex;
	}
	m_pNativeObject = pTerrainObject;
}
auto MapToolRender::TerrainObject::Clone() -> RenderObject^
{
	auto clonedObj{ gcnew TerrainObject{this} };
	return clonedObj;
}

MapToolRender::TerrainObject::TerrainObject(TerrainObject ^ const& rhs):
	RenderObject{ rhs }
{
}
auto MapToolRender::TerrainObject::DiffuseTexture::get()->MapToolRender::Texture^
{
	return m_texture;
}
auto MapToolRender::TerrainObject::DiffuseTexture::set(MapToolRender::Texture^ texture)->void
{
	m_texture = texture;
	static_cast<::TerrainObject*>(this->m_pNativeObject)->SetDiffuseTexture(m_texture->Handle);
	OnPropertyChanged("DiffuseTexture");

}
auto MapToolRender::TerrainObject::Interval::get()->float
{
	float interval = static_cast<::TerrainObject*>(m_pNativeObject)->GetInterval();
	return interval;
}
auto MapToolRender::TerrainObject::Interval::set(float value)->void
{
	static_cast<::TerrainObject*>(m_pNativeObject)->SetInterval(value);
	OnPropertyChanged("Interval");

}
auto MapToolRender::TerrainObject::MaxHeight::get()->float
{
	float height = static_cast<::TerrainObject*>(m_pNativeObject)->GetMaxHeight();
	return height;
}
auto MapToolRender::TerrainObject::MaxHeight::set(float value)->void
{
	static_cast<::TerrainObject*>(m_pNativeObject)->SetMaxHeight(value);
	OnPropertyChanged("MaxHeight");

}
