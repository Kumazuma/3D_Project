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
    // 새로운 트랙에 애니메이션 셋 세팅
    m_pAnimCtrl->SetTrackAnimationSet(newTrackIdx, pAnimSet.Get());
    // 애니메이션 정보 안에 삽입되어 있는 이벤트 정보를 해제하는 함수(이벤트가 없는 것으로 처리)
    // : 우리가 이벤트를 사용할 수 없는 상황인데, 정보가 삽입되어 있는 경우 가끔 애니메이션이 보간 되지 않는 문제가 발생
    m_pAnimCtrl->UnkeyAllTrackEvents(newTrackIdx);
    m_pAnimCtrl->UnkeyAllTrackEvents(currentIdx);
    // 현재 설정된 트랙을 재생 또는 종료 시키기 위한 함수(3인자 : 언제부터 현재 트랙을 해제할 것인가)
    m_pAnimCtrl->KeyTrackEnable(currentIdx, false, m_accTime + 0.25);
    // 인자값으로 들어오는 트랙에 세팅된 애니메이션 셋을 어떤 속도로 움직일 것인지 설정하는 함수(속도의 상수 값은 1)
    m_pAnimCtrl->KeyTrackSpeed(currentIdx, 1.f, m_accTime, 0.25, D3DXTRANSITION_LINEAR);
    // 인자값으로 들어오는 트랙의 가중치를 설정하는 함수
    m_pAnimCtrl->KeyTrackWeight(currentIdx, 0.1f, m_accTime, 0.25, D3DXTRANSITION_LINEAR);
    // New 트랙의 활성화를 지시하는 함수
    m_pAnimCtrl->SetTrackEnable(newTrackIdx, TRUE);
    m_pAnimCtrl->KeyTrackSpeed(newTrackIdx, 1.f, m_accTime, 0.25, D3DXTRANSITION_LINEAR);
    m_pAnimCtrl->KeyTrackWeight(newTrackIdx, 0.9f, m_accTime, 0.25, D3DXTRANSITION_LINEAR);

    m_pAnimCtrl->ResetTime(); // AdvanceTime 호출 시 내부적으로 누적되던 시간을 초기화하는 함수
    m_accTime = 0.f;
    m_oldAnimIdx = idx;
    m_currentTrackIdx = static_cast<i32>(currentIdx);
    m_period = static_cast<f32>(pAnimSet->GetPeriod());
}

auto SkinnedXMeshObject::AnimationController::AdvanceTime(f32 timeDelta) -> void
{
    //m_pAnimCtrl->AdvanceTime(static_cast<f64>(timeDelta), nullptr);
    //하나의 X파일을 여러 오브젝트가 공유하므로 이떄 AdvanceTime을 한다고 해서 의미가 있을 것같지
    //않다. 차라리 함수를 하나 더 만들어서 각 시간때마다의 뼈대의 Transform을 적용시키는 함수를 만드는 것이 나을 것같다.
    // AdvanceTime 호출 시 내부적으로 누적되는 시간 값이 있음
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
