#pragma once

#include <cassert>

#include "RootSignatureBase.h"

class AnimationOutLineMaskRootSignature : public RootSignatureBase
{
public:

	ComPtr<ID3D12RootSignature> BuildRootSignature(ID3D12Device* device, PostEffectType effectType = PostEffectType::kNone) override
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

		D3D12_DESCRIPTOR_RANGE paletteRange[1] = {};
		paletteRange[0].BaseShaderRegister = 1; //3から始まる
		paletteRange[0].NumDescriptors = 1; //数は2つ
		paletteRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
		paletteRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //offsetを自動計算

		D3D12_DESCRIPTOR_RANGE materialRnage[1] = {};
		materialRnage[0].BaseShaderRegister = 0; //3から始まる
		materialRnage[0].NumDescriptors = 1; //数は2つ
		materialRnage[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
		materialRnage[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //offsetを自動計算

		//RootParameterの作成。複数設定できるので配列。今回は結果1つだけなので長さ1の配列
		D3D12_ROOT_PARAMETER rootParameters[5] = {};
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//DescriptorTableを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;				//VertexShaderで使う
		rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;				//Tableの中身の配列を指定
		rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//Tableで利用する数
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//DescriptorTableを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;				//VertexShaderで使う
		rootParameters[1].DescriptorTable.pDescriptorRanges = paletteRange;					//Tableの中身の配列を指定
		rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(paletteRange);		//Tableで利用する数

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					//VertexShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = materialRnage;				//Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(materialRnage);	//Tableで利用する数

		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParameters[3].Descriptor.ShaderRegister = 0;

		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //PixelShaderで使う
		rootParameters[4].Descriptor.ShaderRegister = 1;

		rootSignatureDesc.pParameters = rootParameters;
		rootSignatureDesc.NumParameters = _countof(rootParameters);

		ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
		assert(SUCCEEDED(hr));

		hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(hr));

		return rootSignature;
	}
};
