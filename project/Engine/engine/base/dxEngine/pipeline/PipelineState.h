#pragma once
#include <Windows.h>
#include "wrl.h"
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")

using Microsoft::WRL::ComPtr;

class PipelineState
{
public:

	void Initialize(
		ComPtr<ID3D12Device>& device, ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler, ComPtr<IDxcIncludeHandler> includeHandler);

	/* ==================== Object3d ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateObject3dRootSignature();
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateObject3dPipelineState();

	/* ==================== Sprite ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateSpriteRootSignature();
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateSpritePipelineState();

	/* ==================== Line3d ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateLine3dRootSignature();
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateLine3dPipelineState();

	/* ==================== Particle ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateParticleRootSignature();
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateParticlePipelineState(int blendMode);

	/* ==================== PrimitiveDrawr ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateTrailEffectRootSignature();
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateTrailEffectPipelineState(int blendMode);

	/* ==================== Animation ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateAnimationRootSignature();
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateAnimationPipelineState();

	/* ==================== RenderTexture ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateRenderTextureRootSignature();
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateRenderTexturePipelineState();


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