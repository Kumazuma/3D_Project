#include "FlameEffectShare.hpp"
#include <RenderModule.h>
#include "Env.hpp"

Kumazuma::Client::FlameEffectShareState::FlameEffectShareState(RenderModule* renderModule)
{
	auto base_dir{ Enviroment::GetValue<std::wstring>(Enviroment::BASE_DIR) };
	auto texture1path_{ base_dir + L"/Resource/effect/dragonfireBrew/t_vfx_smokeanim01_blured.png"};
	auto texture2path_{ base_dir + L"/Resource/effect/dragonfireBrew/fire_bright_mod2x_a.png" };
	renderModule->CreateTexture(texture1path_.c_str(), &texture1_);
	renderModule->CreateTexture(texture2path_.c_str(), &texture2_);
}

auto Kumazuma::Client::FlameEffectShareState::Render(
	IDirect3DDevice9* pDevice,
	ID3DXEffect* effect) -> void
{
	effect->SetTexture("g_effectTexture1", texture1_.Get());
	effect->SetTexture("g_effectTexture2", texture2_.Get());
	effect->CommitChanges();
}
