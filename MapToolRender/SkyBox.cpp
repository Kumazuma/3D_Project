#include "pch.h"
#include "SkyBox.h"
#include "Texture.h"
#include <RenderModule/SkyBoxObject.h>
MapToolRender::SkyBox::SkyBox(GraphicsDevice^ device)
{
    SkyBoxObject* pSkyBox{};
    SkyBoxObject::Create(device->Handle, &pSkyBox);
    assert(pSkyBox != nullptr);
    m_pNativeObject = pSkyBox;
}

auto MapToolRender::SkyBox::Clone() -> RenderObject^ 
{
    return gcnew SkyBox(this);
}

MapToolRender::SkyBox::SkyBox(SkyBox^ const& rhs):
    RenderObject(rhs),
    m_texture{rhs->m_texture}
{
    m_pNativeObject = rhs->m_pNativeObject;
}
auto MapToolRender::SkyBox::SkyBoxTexture::get()->Texture^
{
    return m_texture;
}
auto MapToolRender::SkyBox::SkyBoxTexture::set(Texture^ value)->void
{
    static_cast<SkyBoxObject*>(m_pNativeObject)->SetDiffuseTexture(value->Handle);
    m_texture = value;
    OnPropertyChanged("SkyBoxTexture");
}