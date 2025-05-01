#include "PipelineState.h"

#include <cassert>

#include "RootSignatureFactory.h"
#include "InputLayoutFactory.h"
#include "BlendStateFactory.h"
#include "RasterizerStateFactory.h"
#include "CompileShaderFactory.h"
#include "DepthStencilStateFactory.h"

void PipelineState::Initialize(
	ComPtr<ID3D12Device>& device, ComPtr<IDxcUtils> dxcUtils,
	ComPtr<IDxcCompiler3>& dxcCompiler, ComPtr<IDxcIncludeHandler> includeHandler)
{
	device_ = device;
	dxcUtils_ = dxcUtils;
	dxcCompiler_ = dxcCompiler;
	includeHandler_ = includeHandler;
}

ComPtr<ID3D12RootSignature> PipelineState::CreateRootSignature(PipelineType type)
{
	return RootSignatureFactory::GetRootSignature(type, device_);
}

ComPtr<ID3D12PipelineState> PipelineState::CreateObject3dPipelineState()
{
	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = RootSignatureFactory::GetRootSignature(PipelineType::Object3d, device_).Get();
	psoDesc.VS = CompileShaderFactory::GetCompileShader_VS(PipelineType::Object3d, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.PS = CompileShaderFactory::GetCompileShader_PS(PipelineType::Object3d, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::Object3d);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::Object3d);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(BlendMode::kBlendModeNormal);
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = DepthStencilStateFactory::GetDepthStencilState(PipelineType::Object3d);
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateSpritePipelineState()
{
	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = RootSignatureFactory::GetRootSignature(PipelineType::Sprite, device_).Get();
	psoDesc.VS = CompileShaderFactory::GetCompileShader_VS(PipelineType::Sprite, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.PS = CompileShaderFactory::GetCompileShader_PS(PipelineType::Sprite, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::Sprite);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::Sprite);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(BlendMode::kBlendModeNormal);
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = DepthStencilStateFactory::GetDepthStencilState(PipelineType::Sprite);
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateLine3dPipelineState()
{
	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = RootSignatureFactory::GetRootSignature(PipelineType::Line3d, device_).Get();
	psoDesc.VS = CompileShaderFactory::GetCompileShader_VS(PipelineType::Line3d, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.PS = CompileShaderFactory::GetCompileShader_PS(PipelineType::Line3d, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::Line3d);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::Line3d);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(BlendMode::kBlendModeNone);
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = DepthStencilStateFactory::GetDepthStencilState(PipelineType::Line3d);
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateParticlePipelineState(int blendMode)
{
	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = RootSignatureFactory::GetRootSignature(PipelineType::Particle, device_).Get();
	psoDesc.VS = CompileShaderFactory::GetCompileShader_VS(PipelineType::Particle, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.PS = CompileShaderFactory::GetCompileShader_PS(PipelineType::Particle, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::Particle);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::Particle);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(static_cast<BlendMode>(blendMode));
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = DepthStencilStateFactory::GetDepthStencilState(PipelineType::Particle);
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12PipelineState> PipelineState::CreatePrimitiveDrawrPipelineState(int blendMode)
{
	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = RootSignatureFactory::GetRootSignature(PipelineType::PrimitiveDrawr, device_).Get();
	psoDesc.VS = CompileShaderFactory::GetCompileShader_VS(PipelineType::PrimitiveDrawr, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.PS = CompileShaderFactory::GetCompileShader_PS(PipelineType::PrimitiveDrawr, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::PrimitiveDrawr);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::PrimitiveDrawr);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(static_cast<BlendMode>(blendMode));
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = DepthStencilStateFactory::GetDepthStencilState(PipelineType::PrimitiveDrawr);
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateAnimationPipelineState()
{
	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = RootSignatureFactory::GetRootSignature(PipelineType::Animation, device_).Get();
	psoDesc.VS = CompileShaderFactory::GetCompileShader_VS(PipelineType::Animation, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.PS = CompileShaderFactory::GetCompileShader_PS(PipelineType::Animation, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::Animation);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::Animation);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(BlendMode::kBlendModeNormal);
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = DepthStencilStateFactory::GetDepthStencilState(PipelineType::Animation);
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateRenderTexturePipelineState()
{
	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = RootSignatureFactory::GetRootSignature(PipelineType::RenderTexture, device_).Get();
	psoDesc.VS = CompileShaderFactory::GetCompileShader_VS(PipelineType::RenderTexture, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.PS = CompileShaderFactory::GetCompileShader_PS(PipelineType::RenderTexture, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::RenderTexture);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::RenderTexture);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(BlendMode::kBlendModeNone);
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = DepthStencilStateFactory::GetDepthStencilState(PipelineType::RenderTexture);
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}

ComPtr<ID3D12PipelineState> PipelineState::CreateSkyboxPipelineState()
{
	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = RootSignatureFactory::GetRootSignature(PipelineType::Skybox, device_).Get();
	psoDesc.VS = CompileShaderFactory::GetCompileShader_VS(PipelineType::Skybox, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.PS = CompileShaderFactory::GetCompileShader_PS(PipelineType::Skybox, dxcUtils_, dxcCompiler_, includeHandler_);
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(PipelineType::Skybox);
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(PipelineType::Skybox);
	psoDesc.BlendState = BlendStateFactory::GetBlendState(BlendMode::kBlendModeNormal);
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = DepthStencilStateFactory::GetDepthStencilState(PipelineType::Skybox);
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 新しいパイプラインステートオブジェクトの作成
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&newPipelineState_));
	assert(SUCCEEDED(hr_));

	return newPipelineState_;
}
