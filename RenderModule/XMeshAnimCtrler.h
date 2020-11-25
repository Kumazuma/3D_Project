#pragma once
#include "SkinnedXMeshObject.h"
class SkinnedXMeshObject::AnimationController
{
protected:
	AnimationController(ID3DXAnimationController* pAnimCtrl);
	AnimationController(AnimationController const* rhs);
public:
	static auto Create(ID3DXAnimationController* pAniCtrl, AnimationController** pOut)->HRESULT;
	auto Clone()->AnimationController*;
	auto PlayAnimationSet(u32 idx)->void;
	//�ð��� ������Ű�� ����Լ�
	auto AdvanceTime(f32 timeDelta)->void;
	auto IsAnimationSetEnd()->bool;
	//������ ���� ����� �����Ű�� ����Լ�.
	auto AdjustAnimationToFrame()->void;

private:
	COMPtr<ID3DXAnimationController> m_pAnimCtrl;
	i32 m_currentTrackIdx;
	i32 m_oldAnimIdx;
	f32 m_accTime;
	f32 m_period;
	f32 m_lastTimeDelta;
};