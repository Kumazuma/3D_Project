#pragma once
#include "FlameEffect.hpp"
#include <d3d9.h>
#include <d3dx9.h>
class RenderModule;
namespace Kumazuma::Client
{
    class FlameEffectShareState
    {
        friend class FlameEffect;
    public:
        FlameEffectShareState(RenderModule* device);
        auto Render(IDirect3DDevice9* pDevice, ID3DXEffect* effect)->void;
    private:
        COMPtr<IDirect3DTexture9>   texture1_;
        COMPtr<IDirect3DTexture9>   texture2_;
        DWORD fvf_;
    };
}