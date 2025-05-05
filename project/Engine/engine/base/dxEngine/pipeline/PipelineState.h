#pragma once
#include <Windows.h>
#include "wrl.h"
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")

#include "PipelineStruct.h"
#include "PostEffectType.h"

using Microsoft::WRL::ComPtr;

class PipelineState
{
public:

	void Initialize(
		ComPtr<ID3D12Device>& device, ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler, ComPtr<IDxcIncludeHandler> includeHandler);

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateRootSignature(PipelineType type, PostEffectType effectType = PostEffectType::None);

	/* ==================== Object3d ==================== */

	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateObject3dPipelineState();

	/* ==================== Sprite ==================== */

	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateSpritePipelineState();

	/* ==================== Line3d ==================== */

	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateLine3dPipelineState();

	/* ==================== Particle ==================== */

	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateParticlePipelineState(int blendMode);

	/* ==================== PrimitiveDrawr ==================== */

	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreatePrimitiveDrawrPipelineState(int blendMode);

	/* ==================== Animation ==================== */

	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateAnimationPipelineState();

	/* ==================== RenderTexture ==================== */

	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateRenderTexturePipelineState(PostEffectType type = PostEffectType::None);

	/* ==================== Skybox ==================== */

	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateSkyboxPipelineState();


	///==============================================================================================================


private:
	ComPtr<ID3D12Device> device_;
	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;

	ComPtr<ID3D12RootSignature> newRootSignature_;
	ComPtr<ID3D12PipelineState> newPipelineState_;
	HRESULT hr_{};
};