#pragma once
#include <cassert>

#include "RootSignatureBase.h"

class SkinningRootSignature : public RootSignatureBase
{
public:

	ComPtr<ID3D12RootSignature> BuildRootSignature(ID3D12Device* device, PostEffectType effectType = PostEffectType::None) override
	{
		//RootSignature 
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		//DescriptorRange
		D3D12_DESCRIPTOR_RANGE descriptorRange[4][1] = {};
		descriptorRange[0][0].BaseShaderRegister = 0; //3から始まる
		descriptorRange[0][0].NumDescriptors = 1; //数は2つ
		descriptorRange[0][0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
		descriptorRange[0][0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //offsetを自動計算

		descriptorRange[1][0].BaseShaderRegister = 0; //3から始まる
		descriptorRange[1][0].NumDescriptors = 1; //数は2つ
		descriptorRange[1][0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
		descriptorRange[1][0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //offsetを自動計算

		descriptorRange[2][0].BaseShaderRegister = 1; //3から始まる
		descriptorRange[2][0].NumDescriptors = 1; //数は2つ
		descriptorRange[2][0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
		descriptorRange[2][0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //offsetを自動計算

		descriptorRange[3][0].BaseShaderRegister = 2; //3から始まる
		descriptorRange[3][0].NumDescriptors = 1; //数は2つ
		descriptorRange[3][0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
		descriptorRange[3][0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //offsetを自動計算

		//RootParameterの作成。複数設定できるので配列。今回は結果1つだけなので長さ1の配列
		D3D12_ROOT_PARAMETER rootParameters[5] = {};

		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange[1];
		rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange[1]);

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRange[2];
		rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange[2]);

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange[3];
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange[3]);

		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange[0];		
		rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange[0]);

		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[4].Descriptor.ShaderRegister = 0;


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
