#include "COMPickingSphere.hpp"
#include <game/object.hpp>
#include <game/TransformComponent.hpp>
#undef min
#undef max
using namespace DirectX;
IMPLEMENT_COMPONENT_CLASS(Kumazuma::Client::COMPickingSphere);
Kumazuma::Client::COMPickingSphere::COMPickingSphere(f32x3 const& offset, f32 radius):
    Component(TAG),
    offset_{ offset },
    radius_{ radius }
{
}

auto Kumazuma::Client::COMPickingSphere::Clone() const -> Game::Component*
{
    return new COMPickingSphere{ *this };
}

auto Kumazuma::Client::COMPickingSphere::GetRadius() const -> f32
{
    return radius_;
}

auto Kumazuma::Client::COMPickingSphere::GetOffset() const -> f32x3 const&
{
    return offset_;
}

auto Kumazuma::Client::COMPickingSphere::SetRadius(f32 radius) -> void
{
    radius_ = radius;
}

auto Kumazuma::Client::COMPickingSphere::SetOffset(f32x3 const& offset) -> void
{
    offset_ = offset;
}

auto Kumazuma::Client::COMPickingSphere::PickingTest(f32x3 const& at, f32x3 const& dir) const -> std::optional<f32>
{
	auto object{ GetObj().lock() };
	auto transformCom{ std::shared_ptr<Game::TransformComponent const>{} };
	auto transformMatrix{ f32x44{} };
	auto mTransform{ XMMATRIX{} };
	auto radius{ f32{} };
	auto vSphereCenter{ XMVECTOR{} };
	auto vRayAt{ XMVECTOR{} };
	auto vRayDir{ XMVECTOR{} };
	auto vDir{ XMVECTOR{} };
	if (object == nullptr)
		return std::nullopt;
	transformCom = object->GetComponent<Game::TransformComponent>();
	transformCom->GenerateTransform(&transformMatrix);
	mTransform = XMLoadFloat4x4(&transformMatrix);
	XMStoreFloat(&radius, XMVector3Length(XMVector3TransformNormal(XMVectorSet(0.f, 0.f, radius_, 0.f), mTransform)));
	vSphereCenter = XMVector3TransformCoord(XMLoadFloat3(&offset_), mTransform);
	vRayAt = XMLoadFloat3(&at);
	vRayDir = XMVector3Normalize(XMLoadFloat3(&dir));
	vDir = vRayAt - vSphereCenter;

	f32 C{ StoreF32(XMVector3Dot(vDir, vDir)) - radius * radius };
	f32 B{ 2.f * StoreF32(XMVector3Dot(vRayDir , vDir)) };
	f32 discriminant{ B * B - 4.0f * C };
	if (discriminant < 0.f)
	{
		return std::nullopt;
	}

	discriminant = float(sqrt(discriminant));
	float s0{ (-B + discriminant) * 0.5f };
	float s1{ (-B - discriminant) * 0.5f };
	if (s0 < 0.f && s1 < 0.f)
	{
		return std::nullopt;
	}
	if (s0 >= 0.f && s1 >= 0.f)//둘 다 양수일 때
	{
		return std::min(s0, s1);
		//*pOutT = s0 < s1 ? s0 : s1;//작은 놈이 더 가까운 놈
	}
	else//둘 중 하나가 음수(허수) 인 놈이 있을 때
	{
		return std::max(s0, s1);
		//*pOutT = s0 > s1 ? s0 : s1;//큰 놈이 결과값
	}
}
