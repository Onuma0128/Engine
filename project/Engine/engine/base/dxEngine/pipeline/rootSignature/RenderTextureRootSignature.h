#pragma once
#include <vector>
#include <cassert>

#include "RootSignatureBase.h"

class RenderTextureRootSignature : public RootSignatureBase
{
public:

	ComPtr<ID3D12RootSignature> BuildRootSignature(ID3D12Device* device, PostEffectType effectType = PostEffectType::None) override
	{
		//RootSignature 
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		//DescriptorRange
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0; //3から始まる
		descriptorRange[0].NumDescriptors = 1; //数は2つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //offsetを自動計算

		//RootParameterの作成。複数設定できるので配列。今回は結果1つだけなので長さ1の配列
		std::vector<D3D12_ROOT_PARAMETER> rootParameters;

		D3D12_ROOT_PARAMETER param{};
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		param.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		param.DescriptorTable.pDescriptorRanges = descriptorRange;
		param.DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
		rootParameters.push_back(param);

		// 必要ならCreateRootParametersにvectorを渡して編集
		CreateRootParameters(rootParameters, effectType);

		rootSignatureDesc.pParameters = rootParameters.data();
		rootSignatureDesc.NumParameters = static_cast<UINT>(rootParameters.size());

		//Sampler
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; //バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; //ありったけのMipmapを使う
		staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
		rootSignatureDesc.pStaticSamplers = staticSamplers;
		rootSignatureDesc.NumStaticSamplers = _countof(staticSamplers);

		ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
		assert(SUCCEEDED(hr));

		hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(hr));

		return rootSignature;
	}

private:

	void CreateRootParameters(std::vector<D3D12_ROOT_PARAMETER>& rootParameters,PostEffectType type)
	{
		D3D12_ROOT_PARAMETER param{};

		switch (type)
		{
		case PostEffectType::None:
			break;
		case PostEffectType::Grayscale:

			param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			param.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			param.Descriptor.ShaderRegister = 0;
			rootParameters.push_back(param);
			break;
		case PostEffectType::Vignette:

			param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			param.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			param.Descriptor.ShaderRegister = 0;
			rootParameters.push_back(param);
			break;
		case PostEffectType::Blur:
			break;
		case PostEffectType::Invert:
			break;
		case PostEffectType::Noise:
			break;
		case PostEffectType::Bloom:
			break;
		case PostEffectType::Count:
			break;
		default:
			break;
		}
	}
};
