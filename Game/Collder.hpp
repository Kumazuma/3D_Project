#pragma once
#include<DirectXMath.h>
#include<memory>
#include<string>
#include"json.hpp"
namespace Kumazuma::Client
{
	enum class ColliderType : int;
	class ColliderAttribute;
	class Collider
	{
	public:
		Collider();
		Collider(Collider const& rhs);
		Collider(Collider&& rhs) noexcept;
		Collider(nlohmann::json const& data);
		static auto LoadFromJson(nlohmann::json const& data)->Collider*;
		
		auto SetPosition(DirectX::XMFLOAT3 const& val)->void;
		auto SetRotation(DirectX::XMFLOAT3 const& val)->void;
		auto SetScale(DirectX::XMFLOAT3 const& val)->void;
		auto SetOffset(DirectX::XMFLOAT3 const& val)->void;
		auto SetType(ColliderType newType)->void;
		auto SetFrameName(std::wstring const& id)->void;

		auto GetFrameName()const->std::wstring const&;
		auto GetPosition()const->DirectX::XMFLOAT3 const&;
		auto GetRotation()const->DirectX::XMFLOAT3 const&;
		auto GetScale()const->DirectX::XMFLOAT3 const&;
		auto GetOffset()const->DirectX::XMFLOAT3 const&;
		auto GetType()const->ColliderType;
		auto GetAttributeRef()const->ColliderAttribute const&;
		auto GetAttributeRef()->ColliderAttribute&;
		auto GenerateColliderMatrix(DirectX::XMFLOAT4X4* pOut)->void;
		auto CheckCollision(DirectX::XMFLOAT4X4 const& lhsTransform, Collider const& rhs, DirectX::XMFLOAT4X4 const& rhsTransform) ->bool;
	private:
		auto CheckSphere(DirectX::XMFLOAT4X4 const& lhsTransform, Collider const& rhs, DirectX::XMFLOAT4X4 const& rhsTransform)->bool;
		auto CheckOBB(DirectX::XMFLOAT4X4 const& lhsTransform, Collider const& rhs, DirectX::XMFLOAT4X4 const& rhsTransform)->bool;
	private:
		//common property
		std::wstring m_frameName;
		DirectX::XMFLOAT3 m_position;
		DirectX::XMFLOAT3 m_rotation;
		DirectX::XMFLOAT3 m_scale;
		DirectX::XMFLOAT3 m_offset;
		//specify
		std::unique_ptr<ColliderAttribute> m_attribute;
	};
	
}
#include "ColliderAttribute.hpp"