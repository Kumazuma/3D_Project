#include "pch.h"
#include "XMeshAnimCtrler.h"
//COMPtr<ID3DXAnimationController> m_pAnimCtrl;
//i32 m_crreintTrackIdx;
//i32 m_newTrackIdx;
//i32 m_oldAnimIdx;
//f32 m_accTime;
//f64 m_period;
SkinnedXMeshObject::AnimationController::AnimationController(ID3DXAnimationController* pAnimCtrl):
    m_currentTrackIdx{ -1 },
    m_oldAnimIdx{ -1 },
    m_accTime{0.f},
    m_period{ 0.f },
    m_pAnimCtrl{MakeCOMPtr(std::move(pAnimCtrl))}
{
    
}

SkinnedXMeshObject::AnimationController::AnimationController(AnimationController const* rhs):
    m_currentTrackIdx   { rhs->m_currentTrackIdx },
    m_oldAnimIdx{ rhs->m_oldAnimIdx },
    m_accTime{ rhs->m_accTime },
    m_period{ rhs->m_period },
    m_pAnimCtrl{ rhs->m_pAnimCtrl }
{
    ID3DXAnimationSet* animSet{};
    m_pAnimCtrl->GetAnimationSetByName("", &animSet);
 
    m_pAnimCtrl->CloneAnimationController(
        m_pAnimCtrl->GetMaxNumAnimationOutputs(),
        m_pAnimCtrl->GetMaxNumAnimationSets(),
        m_pAnimCtrl->GetMaxNumTracks(),
        m_pAnimCtrl->GetMaxNumEvents(),
        &m_pAnimCtrl
    );
}

auto SkinnedXMeshObject::AnimationController::Create(ID3DXAnimationController* pAnimCtrl, AnimationController** pOut) -> HRESULT
{
    if (pOut == nullptr)
    {
        return E_POINTER;
    }
    
    auto obj = new AnimationController{ pAnimCtrl };
    *pOut = obj;
    return S_OK;
}

auto SkinnedXMeshObject::AnimationController::Clone() -> AnimationController*
{
    return new AnimationController{ this };
}

auto SkinnedXMeshObject::AnimationController::PlayAnimationSet(u32 _idx) -> void
{
    i32 idx{ static_cast<i32>(_idx) };
    if (idx < 0)
    {
        return;
    }
    if (m_oldAnimIdx == idx)
        return;
    u32 newTrackIdx = m_currentTrackIdx != 0 ? 0 : 1;
    u32 currentIdx = m_currentTrackIdx == 0? 0 : 1;
    COMPtr<ID3DXAnimationSet> pAnimSet{};
    m_pAnimCtrl->GetAnimationSet(static_cast<u32>(idx), &pAnimSet);
    // ���ο� Ʈ���� �ִϸ��̼� �� ����
    m_pAnimCtrl->SetTrackAnimationSet(newTrackIdx, pAnimSet.Get());
    // �ִϸ��̼� ���� �ȿ� ���ԵǾ� �ִ� �̺�Ʈ ������ �����ϴ� �Լ�(�̺�Ʈ�� ���� ������ ó��)
    // : �츮�� �̺�Ʈ�� ����� �� ���� ��Ȳ�ε�, ������ ���ԵǾ� �ִ� ��� ���� �ִϸ��̼��� ���� ���� �ʴ� ������ �߻�
    m_pAnimCtrl->UnkeyAllTrackEvents(newTrackIdx);
    m_pAnimCtrl->UnkeyAllTrackEvents(currentIdx);
    // ���� ������ Ʈ���� ��� �Ǵ� ���� ��Ű�� ���� �Լ�(3���� : �������� ���� Ʈ���� ������ ���ΰ�)
    m_pAnimCtrl->KeyTrackEnable(currentIdx, false, m_accTime + 0.25);
    // ���ڰ����� ������ Ʈ���� ���õ� �ִϸ��̼� ���� � �ӵ��� ������ ������ �����ϴ� �Լ�(�ӵ��� ��� ���� 1)
    m_pAnimCtrl->KeyTrackSpeed(currentIdx, 1.f, m_accTime, 0.25, D3DXTRANSITION_LINEAR);
    // ���ڰ����� ������ Ʈ���� ����ġ�� �����ϴ� �Լ�
    m_pAnimCtrl->KeyTrackWeight(currentIdx, 0.1f, m_accTime, 0.25, D3DXTRANSITION_LINEAR);
    // New Ʈ���� Ȱ��ȭ�� �����ϴ� �Լ�
    m_pAnimCtrl->SetTrackEnable(newTrackIdx, TRUE);
    m_pAnimCtrl->KeyTrackSpeed(newTrackIdx, 1.f, m_accTime, 0.25, D3DXTRANSITION_LINEAR);
    m_pAnimCtrl->KeyTrackWeight(newTrackIdx, 0.9f, m_accTime, 0.25, D3DXTRANSITION_LINEAR);

    m_pAnimCtrl->ResetTime(); // AdvanceTime ȣ�� �� ���������� �����Ǵ� �ð��� �ʱ�ȭ�ϴ� �Լ�
    m_accTime = 0.f;
    m_oldAnimIdx = idx;
    m_currentTrackIdx = static_cast<i32>(currentIdx);
    m_period = static_cast<f32>(pAnimSet->GetPeriod());
}

auto SkinnedXMeshObject::AnimationController::AdvanceTime(f32 timeDelta) -> void
{
    //m_pAnimCtrl->AdvanceTime(static_cast<f64>(timeDelta), nullptr);
    //�ϳ��� X������ ���� ������Ʈ�� �����ϹǷ� �̋� AdvanceTime�� �Ѵٰ� �ؼ� �ǹ̰� ���� �Ͱ���
    //�ʴ�. ���� �Լ��� �ϳ� �� ���� �� �ð��������� ������ Transform�� �����Ű�� �Լ��� ����� ���� ���� �Ͱ���.
    // AdvanceTime ȣ�� �� ���������� �����Ǵ� �ð� ���� ����
    m_accTime += timeDelta;
    m_lastTimeDelta += timeDelta;
    //m_accTime = fmodf(m_accTime, m_period);
}

auto SkinnedXMeshObject::AnimationController::IsAnimationSetEnd() -> bool
{
    return m_accTime >= m_period;
}

auto SkinnedXMeshObject::AnimationController::AdjustAnimationToFrame() -> void
{
    m_pAnimCtrl->AdvanceTime(static_cast<f64>(m_lastTimeDelta), nullptr);
    m_lastTimeDelta = 0.f;
}

auto SkinnedXMeshObject::AnimationController::GetAnimCount() const -> u32
{
    return m_pAnimCtrl->GetNumAnimationSets();
}
