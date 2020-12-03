#include "pch.h"
#include "intersact.h"
#include "Common.h"
using namespace DirectX;
auto __vectorcall Intersact(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDir, DirectX::XMVECTOR sphareCenter, f32 radius, f32* pOut) -> bool
{
	XMVECTOR vDir{ rayAt - sphareCenter };
	float C{ StoreVec(XMVector3Dot(vDir, vDir)) - radius * radius };
	float B{ 2.f * StoreVec(XMVector3Dot(rayDir , vDir)) };
	float discriminant{ B * B - 4.0f * C };
	if (discriminant < 0.f)
	{
		return false;
	}

	discriminant = float(sqrt(discriminant));
	float s0{ (-B + discriminant) * 0.5f };
	float s1{ (-B - discriminant) * 0.5f };
	if (s0 < 0.f && s1 < 0.f)
	{
		return false;
	}
	if (s0 >= 0.f && s1 >= 0.f)//�� �� ����� ��
	{
		*pOut = s0 < s1 ? s0 : s1;//���� ���� �� ����� ��
	}
	else//�� �� �ϳ��� ����(���) �� ���� ���� ��
	{
		*pOut = s0 > s1 ? s0 : s1;//ū ���� �����
	}
	return true;
}
