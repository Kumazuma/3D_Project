#include"pch.h"
#include<RenderModule/RenderObject.h>
#include"MapToolRender.h"
#include"Ray.h"
#include<RenderModule/Ray.h>
#pragma unmanaged
void CreateWorldTransform(
	DirectX::XMFLOAT4X4* matrix,
	DirectX::XMFLOAT3 const& position,
	DirectX::XMFLOAT3 const& rotation,
	DirectX::XMFLOAT3 const& scale)
{
	DirectX::XMMATRIX mScale{ DirectX::XMMatrixScaling(scale.x, scale.y,scale.z) };
	DirectX::XMMATRIX mRotation{ DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) };
	DirectX::XMMATRIX mWorld{ mScale * mRotation };
	mWorld.r[3] = DirectX::XMVectorSetW(DirectX::XMLoadFloat3(&position), 1.f);
	DirectX::XMStoreFloat4x4(matrix, mWorld);
}
#pragma managed
namespace MapToolRender
{
	RenderObject::RenderObject():
		m_pNativeObject{},
		m_transform{ gcnew MapToolRender::Transform{}  }
	{
		this->m_transformChangedHandler = gcnew PropertyChangedEventHandler(this, &MapToolRender::RenderObject::OnTransformChanged);
		m_transform->PropertyChanged += m_transformChangedHandler;
	}
	RenderObject::RenderObject(RenderObject^  obj):
		MapObject(obj),
		m_pNativeObject{obj->m_pNativeObject->Clone()},
		m_transform{ gcnew MapToolRender::Transform{obj->m_transform} }
	{
		this->m_transformChangedHandler = gcnew PropertyChangedEventHandler(this, &MapToolRender::RenderObject::OnTransformChanged);
		m_transform->PropertyChanged += m_transformChangedHandler;
	}
	RenderObject::!RenderObject()
	{
		this->~RenderObject();
	}
	RenderObject::~RenderObject()
	{
		if (this->m_pNativeObject != nullptr)
		{
			delete this->m_pNativeObject;
			this->m_pNativeObject = nullptr;
		}
	}
	auto RenderObject::RayPick(Ray^ ray) -> System::Nullable<float>
	{
		float out;
		if (m_pNativeObject->RayPicking(ray->Handle, &out))
		{
			return System::Nullable<float>(out);
		}
		return System::Nullable<float>();
	}
	auto MapToolRender::RenderObject::OnTransformChanged(Object^ obj, PropertyChangedEventArgs^ e) -> void
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT3 rotation;
		position.x = m_transform->Position->X;
		position.y = m_transform->Position->Y;
		position.z = m_transform->Position->Z;

		scale.x = m_transform->Scale->X;
		scale.y = m_transform->Scale->Y;
		scale.z = m_transform->Scale->Z;

		m_transform->Rotation->GetValueToRadian(&rotation);
		DirectX::XMFLOAT4X4 worldMatrix;
		CreateWorldTransform(&worldMatrix, position, rotation, scale);
		m_pNativeObject->SetTransform(worldMatrix);

		OnPropertyChanged("Transform");

	}
	auto RenderObject::Handle::get()->::RenderObject*
	{
		return m_pNativeObject;
	}
	auto RenderObject::Transform::set(MapToolRender::Transform^ value)->void
	{
		m_transform->PropertyChanged -= m_transformChangedHandler;
		m_transform = value;
		m_transform->PropertyChanged += m_transformChangedHandler;
		OnPropertyChanged("Transform");
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT3 rotation;
		position.x = m_transform->Position->X;
		position.y = m_transform->Position->Y;
		position.z = m_transform->Position->Z;

		scale.x = m_transform->Scale->X;
		scale.y = m_transform->Scale->Y;
		scale.z = m_transform->Scale->Z;
		m_transform->Rotation->GetValueToRadian(&rotation);
		DirectX::XMFLOAT4X4 worldMatrix;
		CreateWorldTransform(&worldMatrix, position, rotation, scale);
		m_pNativeObject->SetTransform(worldMatrix);
	}
	auto RenderObject::Transform::get()->MapToolRender::Transform^
	{
		return m_transform;
	}
	auto RenderObject::IsRayPick::get()->bool
	{
		return m_pNativeObject->CanRayPicking();
	}
}