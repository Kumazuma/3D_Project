#include "COMMainUIRender.hpp"
#include <RenderModule.h>
#include <GameRenderer.hpp>
#include <game/object.hpp>
#include "COMCharacterStateInfo.hpp"
#include <game/runtime.hpp>
#include "Env.hpp"
#include <SimplePlane.hpp>
using namespace DirectX;
Kumazuma::Client::COMMainUIRender::COMMainUIRender(std::shared_ptr<Graphics::GameRenderer> const& renderer, std::shared_ptr<Game::Object> const& targetObject):
	hitPoints_(1)
{
	Bind(Game::EVT_UPDATE, &COMMainUIRender::Update);

	COMPtr<IDirect3DDevice9> device;
	renderer->GetDevice(&device);
	targetObject_ = targetObject;
	renderer_ = renderer;
	plane_ = Graphics::SimplePlane::Create(device.Get());
	auto& renderModuleRef{ renderer->GetRenderModuleRef() };
	auto base_dir{ Enviroment::GetValue<std::wstring>(Enviroment::BASE_DIR) };
	auto texture1path_{ base_dir + L"/Resource/ui/ui-player-portrait.png" };
	auto texture2path_{ base_dir + L"/Resource/ui/ui-barfill-simple.png" };
	renderModuleRef.CreateTexture(texture1path_.c_str(), &texture1_);
	renderModuleRef.CreateTexture(texture2path_.c_str(), &texture2_);
	
}

auto Kumazuma::Client::COMMainUIRender::Update(Game::UpdateEvent const& evt) -> void
{
	renderer_->AddUI(std::static_pointer_cast<RenderComponent>(shared_from_this()), 0);
}

auto Kumazuma::Client::COMMainUIRender::PrepareRender(RenderModule& renderModule, Graphics::GameRenderer& render) -> void
{
	auto object{ targetObject_.lock() };
	if (object != nullptr)
	{
		auto stateCom{ object->GetComponent<COMCharacterStateInfo>() };
		if (stateCom != nullptr)
		{
			hitPoints_ = stateCom->GetHitPoints();
		}
	}
}

auto Kumazuma::Client::COMMainUIRender::Render(RenderModule& renderModule, Graphics::GameRenderer& renderer) -> void
{
	COMPtr<ID3DXEffect>			uiEffect;
	COMPtr<IDirect3DDevice9>	device;
	auto const screenWidth{ renderModule.GetWidth() };
	auto const screenHeight{ renderModule.GetHeight() };
	auto const uiWidth{ 256.f };
	auto const uiHeight{ 74.f };
	
	XMMATRIX	mWorldTransform{ }; 
	XMMATRIX	mUVTransform{  };
	f32			hpPercent{ hitPoints_.GetCurrent() / static_cast<f32>(hitPoints_.GetMaximum()) };
	f32 const	hpWidth{ 119.f * hpPercent };
	f32 const	hpOffset{ (119.f - hpWidth) * 0.5f };
	UINT passCount{};
	renderer.GetDevice(&device);
	renderer.GetEffect(L"UI", &uiEffect);
	uiEffect->Begin(&passCount, 0);
	uiEffect->BeginPass(0);
	mWorldTransform = XMMatrixScaling(hpWidth, 10.f, 1.f) * XMMatrixTranslation(uiWidth * 0.5f - screenWidth * 0.5f + 2.f - hpOffset, screenHeight * 0.5f - uiHeight * 0.5f - 2.f, 0.f);
	mUVTransform = XMMatrixIdentity();
	uiEffect->SetTexture("g_uiTexture1", texture2_.Get());
	uiEffect->SetMatrix("g_mWorld", reinterpret_cast<D3DXMATRIX*>(&mWorldTransform));
	uiEffect->SetMatrix("g_mUVTransform1", reinterpret_cast<D3DXMATRIX*>(&mUVTransform));
	uiEffect->CommitChanges();
	plane_->Render(device.Get());

	mWorldTransform = XMMatrixScaling(uiWidth, uiHeight, 1.f) * XMMatrixTranslation(uiWidth * 0.5f - screenWidth * 0.5f, screenHeight * 0.5f - uiHeight * 0.5f, 0.f);
	mUVTransform = XMMatrixScaling(1.f, uiHeight / 128.f, 1.f);

	uiEffect->SetMatrix("g_mWorld", reinterpret_cast<D3DXMATRIX*>(&mWorldTransform));
	uiEffect->SetMatrix("g_mUVTransform1", reinterpret_cast<D3DXMATRIX*>(&mUVTransform));
	uiEffect->SetTexture("g_uiTexture1", texture1_.Get());
	uiEffect->CommitChanges();
	plane_->Render(device.Get());

	uiEffect->EndPass();
	uiEffect->End();
}

auto Kumazuma::Client::COMMainUIRender::Clone() const -> Game::Component*
{
	return nullptr;
}
