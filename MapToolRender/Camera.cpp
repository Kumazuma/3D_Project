#include"pch.h"
#include "Camera.h"
#pragma unmanaged
#pragma warning(push)
#pragma warning(disable: 4793)
void MoveForward(DirectX::XMFLOAT3* pPosition, DirectX::XMFLOAT3* pRotation,  float delta)
{
	using namespace DirectX;
	XMVECTOR vPos{ XMLoadFloat3(pPosition) };
	XMVECTOR vDir{ XMVectorSet(0.f,0.f,1.f,0.f) };
	XMMATRIX mWorldMat{ XMMatrixRotationRollPitchYaw(pRotation->x, pRotation->y, pRotation->z) };
	vDir = XMVector3TransformNormal(vDir, mWorldMat);
	vPos += vDir * delta;
	XMStoreFloat3(pPosition, vPos);
}
void MoveRight(DirectX::XMFLOAT3* pPosition, DirectX::XMFLOAT3* pRotation, float delta)
{
	using namespace DirectX;
	XMVECTOR vPos{ XMLoadFloat3(pPosition) };
	XMVECTOR vDir{ XMVectorSet(1.f,0.f,0.f,0.f) };
	XMMATRIX mWorldMat{ XMMatrixRotationRollPitchYaw(pRotation->x, pRotation->y, pRotation->z) };
	vDir = XMVector3TransformNormal(vDir, mWorldMat);
	vPos += vDir * delta;
	XMStoreFloat3(pPosition, vPos);
}
void MoveUp(DirectX::XMFLOAT3* pPosition, DirectX::XMFLOAT3* pRotation, float delta)
{
	using namespace DirectX;
	XMVECTOR vPos{ XMLoadFloat3(pPosition) };
	XMVECTOR vDir{ XMVectorSet(0.f,1.f,0.f,0.f) };
	XMMATRIX mWorldMat{ XMMatrixRotationRollPitchYaw(pRotation->x, pRotation->y, pRotation->z) };
	vDir = XMVector3TransformNormal(vDir, mWorldMat);
	vPos += vDir * delta;
	XMStoreFloat3(pPosition, vPos);
}
#pragma warning(pop)
#pragma managed
MapToolRender::Camera::Camera()
{
	m_pPosition = new DirectX::XMFLOAT3{ 0.f, 0.f, 0.f };
	m_pRotation = new DirectX::XMFLOAT3{ 0.f, 0.f, 0.f };
	m_position = gcnew MapToolRender::Position;
	m_rotation = gcnew MapToolRender::Rotation;
}

MapToolRender::Camera::~Camera()
{
	if (m_pPosition != nullptr)
	{
		delete m_pPosition;
	}
	if (m_pRotation != nullptr)
	{
		delete m_pRotation;
	}
}

MapToolRender::Camera::!Camera()
{
	this->~Camera();
}

auto MapToolRender::Camera::MoveUp(float delta) -> void
{
	::MoveUp(m_pPosition, m_pRotation, delta);
	m_position->X = m_pPosition->x;
	m_position->Y = m_pPosition->y;
	m_position->Z = m_pPosition->z;
}

auto MapToolRender::Camera::MoveForward(float delta) -> void
{
	::MoveForward(m_pPosition, m_pRotation, delta);
	m_position->X = m_pPosition->x;
	m_position->Y = m_pPosition->y;
	m_position->Z = m_pPosition->z;
}

auto MapToolRender::Camera::MoveRight(float delta) -> void
{
	::MoveRight(m_pPosition, m_pRotation, delta);
	m_position->X = m_pPosition->x;
	m_position->Y = m_pPosition->y;
	m_position->Z = m_pPosition->z;

}
auto MapToolRender::Camera::RotationX(float delta) -> void
{
	m_rotation->X += delta;
}
auto MapToolRender::Camera::RotationY(float delta) -> void
{
	m_rotation->Y += delta;
}
auto MapToolRender::Camera::Position::get()->MapToolRender::Position^
{
	return m_position;
}
auto MapToolRender::Camera::Position::set(MapToolRender::Position^ value)->void
{
	m_position = value;
	m_pPosition->x = value->X;
	m_pPosition->y = value->Y;
	m_pPosition->z = value->Z;
}
auto MapToolRender::Camera::Rotation::get()->MapToolRender::Rotation^
{
	return m_rotation;
}
auto MapToolRender::Camera::Rotation::set(MapToolRender::Rotation^ value)->void
{
	m_rotation = value;
	m_rotation->GetValueToRadian(m_pRotation);
}
auto MapToolRender::Camera::RotationPtr::get()->DirectX::XMFLOAT3*
{
	m_rotation->GetValueToRadian(m_pRotation);
	return m_pRotation;
}
auto MapToolRender::Camera::PositionPtr::get()->DirectX::XMFLOAT3*
{
	return m_pPosition;
}