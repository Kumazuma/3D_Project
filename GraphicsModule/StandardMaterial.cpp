#include "pch.h"
#include "StandardMaterial.hpp"
#include "GraphicsModule.hpp"
#include "RenderSystem.hpp"
#include "Texture2D.hpp"
#include <wrl.h>
using namespace Microsoft::WRL;
namespace Kumazuma
{
	StandardMaterial::StandardMaterial(GraphicsModule* gmodule, Mesh const* mesh):
		Material{MaterialShadingClass::DeferredShading},
		gmodule_{ gmodule },
		mesh_{ mesh },
		worldMatrixPtr_{nullptr},
		pixelShader_{}
	{
		float borderColor[4]{ 0.f,0.f,0.f,0.f };
		gmodule_->GetPixelShader(L"deferred_gbuffer_ps",  &pixelShader_);
		gmodule_->CreateCBuffer(L"deferred_gbuffer_material", sizeof(DirectX::XMFLOAT4));
		gmodule_->GetCBuffer(L"deferred_gbuffer_material", &materialCBuffer_);
		D3D11_SAMPLER_DESC samplerDesc{};;
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
		if (materialCBuffer_ != nullptr)
		{
			materialCBuffer_->Release();
			materialCBuffer_ = nullptr;
		}
	}
	void StandardMaterial::SetWorldMatrixPtr(DirectX::XMFLOAT4X4* ptr)
	{
		worldMatrixPtr_ = ptr;
	}
	void StandardMaterial::RenderShadowMap()
	{

	}
	void StandardMaterial::Render(RenderSystem* renderSystem, ID3D11DeviceContext* deviceContext)
	{
		ID3D11ShaderResourceView* srv{};
		ID3D11SamplerState* samplerState{ samplerState_ };
		auto collection{ mesh_->GetMeshCollection() };
		auto* texture2D{ collection->GetMaterialTexture(mesh_->GetMaterial().c_str()) };
		if (texture2D != nullptr)
		{
			texture2D->GetView(&srv);
		}
		renderSystem->SettupVertexShader(mesh_->GetType(), deviceContext,worldMatrixPtr_);
		mesh_->SetupIA(deviceContext);
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			HRESULT hr = deviceContext->Map(materialCBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			*((DirectX::XMFLOAT4*)mappedResource.pData) = DirectX::XMFLOAT4{0.f, 0.f, 0.f, 0.f };
			deviceContext->Unmap(materialCBuffer_, 0);
		}
		deviceContext->PSSetShader(pixelShader_, nullptr, 0);
		deviceContext->PSSetSamplers(0, 1, &samplerState);
		deviceContext->PSSetShaderResources(0, 1, &srv);
		deviceContext->PSSetConstantBuffers(0, 1, &materialCBuffer_);
		deviceContext->DrawIndexed(mesh_->GetIndexCount(), mesh_->GetIndexBase(), 0);
		if (srv != nullptr)
		{
			srv->Release();
		}
	}
}
