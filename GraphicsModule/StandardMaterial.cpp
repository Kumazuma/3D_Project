#include "pch.h"
#include "StandardMaterial.hpp"
#include "GraphicsModule.hpp"
#include "RenderSystem.hpp"
#include "Texture2D.hpp"
#include "Mesh.hpp"
#include <wrl.h>
using namespace Microsoft::WRL;
namespace Kumazuma
{
	StandardMaterial::StandardMaterial(GraphicsModule* gmodule, Subset const* subset):
		Material{MaterialShadingClass::DeferredShading},
		gmodule_{ gmodule },
		subset_{ subset },
		worldMatrixPtr_{nullptr}
	{
		float borderColor[4]{ 0.f,0.f,0.f,0.f };
		gmodule_->GetPixelShader(L"deferred_gbuffer_ps",  &pixelShader_);
		D3D11_SAMPLER_DESC samplerDesc{};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		ComPtr<ID3D11Device> device;
		gmodule_->GetDevice(&device);
		device->CreateSamplerState(&samplerDesc, &samplerState_);
	}
	StandardMaterial::~StandardMaterial()
	{
		if (pixelShader_ != nullptr)
		{
			pixelShader_->Release();
			pixelShader_ = nullptr;
		}
		if (samplerState_ != nullptr)
		{
			samplerState_->Release();
			samplerState_ = nullptr;
		}
	}
	void StandardMaterial::SetWorldMatrixPtr(DirectX::XMFLOAT4X4* ptr)
	{
		worldMatrixPtr_ = ptr;
	}
	void StandardMaterial::RenderShadowMap()
	{

	}
	void StandardMaterial::Render(ID3D11DeviceContext* deviceContext)
	{
		ID3D11ShaderResourceView* srv{};
		ID3D11SamplerState* samplerState{ samplerState_ };
		auto& renderSystem{ gmodule_->GetRenderSystem() };
		auto& mesh{ subset_->GetMesh() };
		auto* texture2D{ mesh.GetMaterialTexture(subset_->GetMaterial().c_str()) };
		if (texture2D != nullptr)
		{
			texture2D->GetView(&srv);
		}
		renderSystem.SettupVertexShader(MeshType::Static, deviceContext,worldMatrixPtr_);
		mesh.SetupIA(deviceContext);
		deviceContext->PSSetShader(pixelShader_, nullptr, 0);
		deviceContext->PSSetSamplers(0, 1, &samplerState);
		deviceContext->PSSetShaderResources(0, 1, &srv);
		deviceContext->DrawIndexed(subset_->GetTriangleCount() * 3, subset_->GetIndexBase(), 0);
		if (srv != nullptr)
		{
			srv->Release();
		}
	}
}
