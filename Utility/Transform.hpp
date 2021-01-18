#pragma once
#include<DirectXMath.h>
namespace Kumazuma
{
	struct Transform
	{
		DirectX::XMFLOAT3 rotation;
		DirectX::XMFLOAT3 position;
		float scale;
	};
}