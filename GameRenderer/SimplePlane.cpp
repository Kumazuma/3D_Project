#include "pch.h"
#include "include/SimplePlane.hpp"
#include "SimplePlaneImpl.hpp"

auto Kumazuma::Graphics::SimplePlane::Create(IDirect3DDevice9* device) -> std::unique_ptr<SimplePlane>
{
    return std::unique_ptr<SimplePlane>(new SimplePlaneImpl{ device });
}
