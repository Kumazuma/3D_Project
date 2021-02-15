#include "pch.h"
#include "Texture2D.hpp"
#include "Texture2DImpl.hpp"
namespace Kumazuma
{
	Texture2D* Texture2D::Create(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& desc)
	{
		try
		{
			Texture2DImpl tex2D{ device, desc };
			return new Texture2DImpl{ tex2D };
		}
		catch (...)
		{

		}
		return nullptr;
	}
}

