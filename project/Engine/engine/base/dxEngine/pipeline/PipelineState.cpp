#include "PipelineState.h"

#include <cassert>

#include "PipelineStruct.h"
#include "InputLayoutFactory.h"
#include "BlendStateFactory.h"
#include "RasterizerStateFactory.h"

#include "CompileShader.h"

void PipelineState::Initialize(
	ComPtr<ID3D12Device>& device, ComPtr<IDxcUtils> dxcUtils,
	ComPtr<IDxcCompiler3>& dxcCompiler, ComPtr<IDxcIncludeHandler> includeHandler)
{
	device_ = device;
	dxcUtils_ = dxcUtils;
	dxcCompiler_ = dxcCompiler;
	includeHandler_ = includeHandler;
}

void PipelineState::Object3dShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& geometryShader, ComPtr<IDxcBlob>& pixelShader)
{
	vertexShader = CompileShader(L"resources/shaders/Object3d.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vertexShader != nullptr);

	geometryShader = CompileShader(L"resources/shaders/BasicGeometryShader.hlsl", L"gs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(geometryShader != nullptr);

	pixelShader = CompileShader(L"resources/shaders/Object3d.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(pixelShader != nullptr);
}

void PipelineState::Line3dShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader)
{
	vertexShader = CompileShader(L"resources/shaders/Line3d.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vertexShader != nullptr);

	pixelShader = CompileShader(L"resources/shaders/Line3d.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(pixelShader != nullptr);
}

void PipelineState::ParticleShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader)
{
	vertexShader = CompileShader(L"resources/shaders/Particle.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vertexShader != nullptr);

	pixelShader = CompileShader(L"resources/shaders/Particle.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(pixelShader != nullptr);
}

void PipelineState::Object3dDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc)
{
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

void PipelineState::SpriteDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc)
{
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

void PipelineState::ParticleDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc)
{
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

ComPtr<ID3D12RootSignature> PipelineState::CreateObject3dRootSignature()
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
	D3D12_ROOT_PARAMETER rootParameters[7] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; //Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); //Tableで利用する数
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[3].Descriptor.ShaderRegister = 1;
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[4].Descriptor.ShaderRegister = 2;
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[5].Descriptor.ShaderRegister = 3;
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[6].Descriptor.ShaderRegister = 4;


	rootSignatureDesc.pParameters = rootParameters;
	rootSignatureDesc.NumParameters = _countof(rootParameters);
	//Sampler
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; //バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; //ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootSignatureDesc.pStaticSamplers = staticSamplers;
	rootSignatureDesc.NumStaticSamplers = _countof(staticSamplers);

	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	hr_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	assert(SUCCEEDED(hr_));

	hr_ = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&newRootSignature_));
	assert(SUCCEEDED(hr_));

	return newRootSignature_;
}

ComPtr<ID3D12RootSignature> PipelineState::CreateSpriteRootSignature()
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
	D3D12_ROOT_PARAMETER rootParameters[7] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; //Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); //Tableで利用する数
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[3].Descriptor.ShaderRegister = 1;
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[4].Descriptor.ShaderRegister = 2;
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[5].Descriptor.ShaderRegister = 3;
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[6].Descriptor.ShaderRegister = 4;


	rootSignatureDesc.pParameters = rootParameters;
	rootSignatureDesc.NumParameters = _countof(rootParameters);
	//Sampler
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; //バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; //ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootSignatureDesc.pStaticSamplers = staticSamplers;
	rootSignatureDesc.NumStaticSamplers = _countof(staticSamplers);

	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	hr_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	assert(SUCCEEDED(hr_));

	hr_ = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&newRootSignature_));
	assert(SUCCEEDED(hr_));

	return newRootSignature_;
}

ComPtr<ID3D12RootSignature> PipelineState::CreateLine3dRootSignature()
{
	//RootSignature 
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//RootParameterの作成。複数設定できるので配列。今回は結果1つだけなので長さ1の配列
	D3D12_ROOT_PARAMETER rootParameters[1] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //VertexShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;

	rootSignatureDesc.pParameters = rootParameters;
	rootSignatureDesc.NumParameters = _countof(rootParameters);

	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	hr_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	assert(SUCCEEDED(hr_));

	hr_ = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&newRootSignature_));
	assert(SUCCEEDED(hr_));

	return newRootSignature_;
}

ComPtr<ID3D12RootSignature> PipelineState::CreateParticleRootSignature()
{
	// 新しいルートシグネチャの作成
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; //3から始まる
	descriptorRange[0].NumDescriptors = 1; //数は2つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //offsetを自動計算

	D3D12_ROOT_PARAMETER rootParameters[4]{};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	rootSignatureDesc.pParameters = rootParameters;
	rootSignatureDesc.NumParameters = _countof(rootParameters);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; //バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; //ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う

	rootSignatureDesc.pStaticSamplers = staticSamplers;
	rootSignatureDesc.NumStaticSamplers = _countof(staticSamplers);

	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	hr_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	assert(SUCCEEDED(hr_));

	hr_ = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&newRootSignature_));
	assert(SUCCEEDED(hr_));

	return newRootSignature_;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateObject3dPipelineState()
{
	// シェーダーのコンパイル
	ComPtr<IDxcBlob> vertexShaderBlob;
	ComPtr<IDxcBlob> geometryShaderBlob;
	ComPtr<IDxcBlob> pixelShaderBlob;
	Object3dShader(vertexShaderBlob, geometryShaderBlob, pixelShaderBlob);

	// デスクリプターステンシル
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	Object3dDepthStencilState(depthStencilDesc);

	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = newRootSignature_.Get();
	psoDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
	psoDesc.GS = { geometryShaderBlob->GetBufferPointer(),geometryShaderBlob->GetBufferSize() };
	psoDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::Object3d);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::Object3d);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(BlendMode::kBlendModeNormal);
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = depthStencilDesc;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateSpritePipelineState()
{
	// シェーダーのコンパイル
	ComPtr<IDxcBlob> vertexShaderBlob;
	ComPtr<IDxcBlob> geometryShaderBlob;
	ComPtr<IDxcBlob> pixelShaderBlob;
	Object3dShader(vertexShaderBlob, geometryShaderBlob, pixelShaderBlob);

	// デスクリプターステンシル
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	SpriteDepthStencilState(depthStencilDesc);

	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = newRootSignature_.Get();
	psoDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
	psoDesc.GS = { geometryShaderBlob->GetBufferPointer(),geometryShaderBlob->GetBufferSize() };
	psoDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::Sprite);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::Sprite);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(BlendMode::kBlendModeNormal);
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = depthStencilDesc;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateLine3dPipelineState()
{
	// シェーダーのコンパイル
	ComPtr<IDxcBlob> vertexShaderBlob;
	ComPtr<IDxcBlob> pixelShaderBlob;
	Line3dShader(vertexShaderBlob, pixelShaderBlob);

	// デスクリプターステンシル
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	Object3dDepthStencilState(depthStencilDesc);

	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = newRootSignature_.Get();
	psoDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
	psoDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::Line3d);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::Line3d);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(BlendMode::kBlendModeNone);
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = depthStencilDesc;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateParticlePipelineState(int blendMode)
{
	// シェーダーのコンパイル
	ComPtr<IDxcBlob> vertexShaderBlob;
	ComPtr<IDxcBlob> pixelShaderBlob;
	ParticleShader(vertexShaderBlob, pixelShaderBlob);

	// デスクリプターステンシル
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	ParticleDepthStencilState(depthStencilDesc);

	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = newRootSignature_.Get();
	psoDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
	psoDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::Particle);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::Particle);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(static_cast<BlendMode>(blendMode));
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = depthStencilDesc;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12RootSignature> PipelineState::CreateTrailEffectRootSignature()
{
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; //3から始まる
	descriptorRange[0].NumDescriptors = 1; //数は2つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //offsetを自動計算

	//RootParameterの作成。複数設定できるので配列。今回は結果1つだけなので長さ1の配列
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //VertexShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

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
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	hr_ = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	assert(SUCCEEDED(hr_));

	// バイナリを元に戻す
	hr_ = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&newRootSignature_));
	assert(SUCCEEDED(hr_));

	return newRootSignature_;
}

void PipelineState::TrailEffectShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader)
{
	vertexShader = CompileShader(L"resources/shaders/PrimitiveDrawr.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vertexShader != nullptr);

	pixelShader = CompileShader(L"resources/shaders/PrimitiveDrawr.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(pixelShader != nullptr);
}

void PipelineState::TrailEffectDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc)
{
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みしない
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateTrailEffectPipelineState(int blendMode)
{
	// シェーダーのコンパイル
	ComPtr<IDxcBlob> vertexShaderBlob;
	ComPtr<IDxcBlob> pixelShaderBlob;
	TrailEffectShader(vertexShaderBlob, pixelShaderBlob);

	// デスクリプターステンシル
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	TrailEffectDepthStencilState(depthStencilDesc);

	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = newRootSignature_.Get();
	psoDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
	psoDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::PrimitiveDrawr);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::PrimitiveDrawr);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(static_cast<BlendMode>(blendMode));
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = depthStencilDesc;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12RootSignature> PipelineState::CreateAnimationRootSignature()
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

	D3D12_DESCRIPTOR_RANGE matrixPalette[1] = {};
	matrixPalette[0].BaseShaderRegister = 1; //3から始まる
	matrixPalette[0].NumDescriptors = 1; //数は2つ
	matrixPalette[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
	matrixPalette[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //offsetを自動計算

	//RootParameterの作成。複数設定できるので配列。今回は結果1つだけなので長さ1の配列
	D3D12_ROOT_PARAMETER rootParameters[8] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; //Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); //Tableで利用する数

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[3].DescriptorTable.pDescriptorRanges = matrixPalette; //Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(matrixPalette); //Tableで利用する数

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[4].Descriptor.ShaderRegister = 1;
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[5].Descriptor.ShaderRegister = 2;
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[6].Descriptor.ShaderRegister = 3;
	rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootParameters[7].Descriptor.ShaderRegister = 4;


	rootSignatureDesc.pParameters = rootParameters;
	rootSignatureDesc.NumParameters = _countof(rootParameters);
	//Sampler
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; //バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; //ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	rootSignatureDesc.pStaticSamplers = staticSamplers;
	rootSignatureDesc.NumStaticSamplers = _countof(staticSamplers);

	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	hr_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	assert(SUCCEEDED(hr_));

	hr_ = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&newRootSignature_));
	assert(SUCCEEDED(hr_));

	return newRootSignature_;
}

void PipelineState::AnimationShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& geometryShader, ComPtr<IDxcBlob>& pixelShader)
{
	vertexShader = CompileShader(L"resources/shaders/SkinningObject3d.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vertexShader != nullptr);

	geometryShader = CompileShader(L"resources/shaders/BasicGeometryShader.hlsl", L"gs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(geometryShader != nullptr);

	pixelShader = CompileShader(L"resources/shaders/Object3d.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(pixelShader != nullptr);
}

void PipelineState::AnimationDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc)
{
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateAnimationPipelineState()
{
	// シェーダーのコンパイル
	ComPtr<IDxcBlob> vertexShaderBlob;
	ComPtr<IDxcBlob> geometryShaderBlob;
	ComPtr<IDxcBlob> pixelShaderBlob;
	AnimationShader(vertexShaderBlob, geometryShaderBlob, pixelShaderBlob);

	// デスクリプターステンシル
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	AnimationDepthStencilState(depthStencilDesc);

	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = newRootSignature_.Get();
	psoDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
	psoDesc.GS = { geometryShaderBlob->GetBufferPointer(),geometryShaderBlob->GetBufferSize() };
	psoDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::Animation);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::Animation);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(BlendMode::kBlendModeNormal);
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = depthStencilDesc;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12RootSignature> PipelineState::CreateRenderTextureRootSignature()
{

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; //3から始まる
	descriptorRange[0].NumDescriptors = 1; //数は2つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //offsetを自動計算

	//RootParameterの作成。複数設定できるので配列。今回は結果1つだけなので長さ1の配列
	D3D12_ROOT_PARAMETER rootParameters[1] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //VertexShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

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
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	hr_ = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	assert(SUCCEEDED(hr_));

	// バイナリを元に戻す
	hr_ = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&newRootSignature_));
	assert(SUCCEEDED(hr_));

	return newRootSignature_;
}

void PipelineState::RenderTextureShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader)
{
	vertexShader = CompileShader(L"resources/shaders/CopyImage.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(vertexShader != nullptr);
	pixelShader = CompileShader(L"resources/shaders/CopyImage.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(pixelShader != nullptr);
}

void PipelineState::RenderTextureDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc)
{
	//Depthの機能を有効化にしない
	depthStencilDesc.DepthEnable = false;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateRenderTexturePipelineState()
{
	// シェーダーのコンパイル
	ComPtr<IDxcBlob> vertexShaderBlob;
	ComPtr<IDxcBlob> pixelShaderBlob;
	RenderTextureShader(vertexShaderBlob, pixelShaderBlob);

	// デスクリプターステンシル
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	RenderTextureDepthStencilState(depthStencilDesc);

	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = newRootSignature_.Get();
	psoDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
	psoDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::RenderTexture);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::RenderTexture);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(BlendMode::kBlendModeNone);
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = depthStencilDesc;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}
