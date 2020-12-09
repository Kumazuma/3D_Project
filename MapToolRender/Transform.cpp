#include "pch.h"
#include "Transform.h"

MapToolRender::Transform::Transform()
{
    this->rotation = MapToolCore::Rotation(0.f, 0.f, 0.f);
    this->scale = MapToolCore::Scale(1.f, 1.f, 1.f);
    this->position =  MapToolCore::Position(0.f, 0.f, 0.f);


}
MapToolRender::Transform::Transform(Transform^  rhs)
{
    this->rotation = MapToolCore::Rotation(rhs->rotation.X, rhs->rotation.Y, rhs->rotation.Z);
    this->scale = MapToolCore::Scale(rhs->scale.X, rhs->scale.Y, rhs->scale.Z);
    this->position =  MapToolCore::Position( rhs->position.X, rhs->position.Y, rhs->position.Z);

}
auto MapToolRender::Transform::Clone() -> Transform^
{
    return gcnew Transform(this);
}
auto MapToolRender::Transform::Rotation::get()->MapToolCore::Rotation
{
    return rotation;
}
auto MapToolRender::Transform::Rotation::set(MapToolCore::Rotation value)->void
{
    rotation = value;
    OnPropertyChanged(L"Rotation");
}
auto MapToolRender::Transform::Position::get()->MapToolCore::Position
{
    return position;
}
auto MapToolRender::Transform::Position::set(MapToolCore::Position value)->void
{
    position = value;
    OnPropertyChanged(L"Position");
}
auto MapToolRender::Transform::Scale::set(MapToolCore::Scale value)->void
{
    scale = value;
    OnPropertyChanged(L"Scale");
}
auto MapToolRender::Transform::Scale::get()->MapToolCore::Scale
{
    return scale;
}
void MapToolRender::Transform::OnChangedRotation(Object^ obj, PropertyChangedEventArgs^ e)
{
    OnPropertyChanged("Rotation");
}

void MapToolRender::Transform::OnChangedScale(Object^ obj, PropertyChangedEventArgs^ e)
{
    OnPropertyChanged("Scale");
}

void MapToolRender::Transform::OnChangedPosition(Object^ obj, PropertyChangedEventArgs^ e)
{
    OnPropertyChanged("Position");
}

#pragma unmanaged
#include<DirectXMath.h>
auto GetValueToRadian(float x, float y, float z, DirectX::XMFLOAT3* pOut)->void
{
    using namespace DirectX;
    XMVECTOR vRotation{ XMVectorSet(x,y,z,0.f) };
    vRotation = vRotation * DirectX::XM_PI / 180.f;
    XMStoreFloat3(pOut, vRotation);
}
#pragma managed
//auto MapToolRender::Rotation::GetValueToRadian(DirectX::XMFLOAT3* pOut) -> void
//{
//    ::GetValueToRadian(x, y, z, pOut);
//}
