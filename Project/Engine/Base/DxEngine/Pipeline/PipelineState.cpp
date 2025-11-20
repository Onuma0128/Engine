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

	// 必要な全ての組み合わせで初期化（例：ポストエフェクト含む）
	std::vector<PipelineType> pipelineTypes = {
		PipelineType::kObject3d,		PipelineType::kSprite,		PipelineType::kLine3d,
		PipelineType::kParticle,		PipelineType::kPrimitiveDrawr,
		PipelineType::kAnimation,	PipelineType::kRenderTexture,
		PipelineType::kSkybox,		PipelineType::kObjectOutLineMask,
		PipelineType::kAnimationOutLineMask,
		PipelineType::kObjectShadowMapDepth,
		PipelineType::kAnimationShadowMapDepth
	};

	std::vector<PostEffectType> postEffectTypes = {
		PostEffectType::kNone, PostEffectType::kRenderTexture,
		PostEffectType::kGrayscale, PostEffectType::kVignette,
		PostEffectType::kSmoothing, PostEffectType::kOutLine,
		PostEffectType::kOutLineMask
	};

	// RootSignature,Pipelineの作成
	for (auto type : pipelineTypes) {
		for (auto effect : postEffectTypes) {
			// Effectありで必要なものだけ処理（RenderTexture等）
			if ((effect != PostEffectType::kNone && type != PipelineType::kRenderTexture) ||
				(effect == PostEffectType::kNone && type == PipelineType::kRenderTexture)) {
				continue; 
			}
			// BlendMode複数持ちの設定
			if (type == PipelineType::kPrimitiveDrawr || type == PipelineType::kParticle) {
				for (int blend = 0; blend < static_cast<int>(BlendMode::kCount); ++blend) {
					PipelineKey key{ type, effect, static_cast<BlendMode>(blend) };
					rootSignatures_[key] = CreateRootSignature(type, effect);
					pipelineStates_[key] = CreatePipelineState(type, effect, static_cast<BlendMode>(blend));
				}
			// その他設定
			} else {
				if (type == PipelineType::kRenderTexture || type == PipelineType::kObjectOutLineMask|| 
					type == PipelineType::kAnimationOutLineMask) {
					PipelineKey key{ type, effect, BlendMode::kBlendModeNone };
					rootSignatures_[key] = CreateRootSignature(type, effect);
					pipelineStates_[key] = CreatePipelineState(type, effect, BlendMode::kBlendModeNone);
				} else {
					PipelineKey key{ type, effect, BlendMode::kBlendModeNormal };
					rootSignatures_[key] = CreateRootSignature(type, effect);
					pipelineStates_[key] = CreatePipelineState(type, effect, BlendMode::kBlendModeNormal);
				}
			}
		}
	}
}

ComPtr<ID3D12RootSignature> PipelineState::CreateRootSignature(PipelineType type,PostEffectType effectType)
{
	if (effectType != PostEffectType::kNone) {
		return RootSignatureFactory::GetRootSignature(type, device_, effectType);
	} else {
		return RootSignatureFactory::GetRootSignature(type, device_);
	}
}

ComPtr<ID3D12PipelineState> PipelineState::CreatePipelineState(PipelineType type, PostEffectType effectType, BlendMode blendMode)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
	psoDesc.pRootSignature = GetRootSignature(type, effectType, blendMode).Get();
	psoDesc.InputLayout = InputLayoutFactory::GetInputLayout(type);
	psoDesc.PrimitiveTopologyType = (type == PipelineType::kLine3d) ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE : D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// シェーダー設定
	if (type == PipelineType::kObjectShadowMapDepth ||
		type == PipelineType::kAnimationShadowMapDepth) {
		psoDesc.VS = CompileShaderFactory::GetCompileShader_VS(type, dxcUtils_, dxcCompiler_, includeHandler_);
		psoDesc.PS = { nullptr, 0 };
		psoDesc.NumRenderTargets = 0;
		psoDesc.BlendState = BlendStateFactory::GetBlendState(BlendMode::kBlendModeNone);
	} else {
		psoDesc.VS = CompileShaderFactory::GetCompileShader_VS(type, dxcUtils_, dxcCompiler_, includeHandler_);
		psoDesc.PS = CompileShaderFactory::GetCompileShader_PS(type, dxcUtils_, dxcCompiler_, includeHandler_, effectType);
		if (type == PipelineType::kObjectOutLineMask || 
			type == PipelineType::kAnimationOutLineMask) {
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			psoDesc.RTVFormats[1] = DXGI_FORMAT_R32_UINT;
			psoDesc.NumRenderTargets = 2;
		} else {
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			psoDesc.NumRenderTargets = 1;
		}
		psoDesc.BlendState = BlendStateFactory::GetBlendState(blendMode);
	}
	// 既存の同じ設定
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.RasterizerState = RasterizerStateFactory::GetRasterizerDesc(type);
	psoDesc.DepthStencilState = DepthStencilStateFactory::GetDepthStencilState(type);
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	ComPtr<ID3D12PipelineState> pipelineState;
	hr_ = device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(hr_));
	return pipelineState;
}

ComPtr<ID3D12RootSignature> PipelineState::GetRootSignature(PipelineType type, PostEffectType effectType, BlendMode blend)
{
	PipelineKey key{ type, effectType, blend };
	return rootSignatures_.at(key);
}

ComPtr<ID3D12PipelineState> PipelineState::GetPipelineState(PipelineType type, PostEffectType effectType, BlendMode blend)
{
	PipelineKey key{ type, effectType, blend };
	return pipelineStates_.at(key);
}
