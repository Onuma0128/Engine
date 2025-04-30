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
	// シェーダーコンパイル
	void Object3dShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& geometryShader, ComPtr<IDxcBlob>& pixelShader);
	// デスクリプターステンシル
	void Object3dDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateObject3dPipelineState();

	/* ==================== Sprite ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateSpriteRootSignature();
	// デスクリプターステンシル
	void SpriteDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateSpritePipelineState();

	/* ==================== Line3d ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateLine3dRootSignature();
	// シェーダーコンパイル
	void Line3dShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader);
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateLine3dPipelineState();

	/* ==================== Particle ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateParticleRootSignature();
	// シェーダーコンパイル
	void ParticleShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader);
	// デスクリプターステンシル
	void ParticleDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateParticlePipelineState(int blendMode);

	/* ==================== PrimitiveDrawr ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateTrailEffectRootSignature();
	// シェーダーコンパイル
	void TrailEffectShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader);
	// デスクリプターステンシル
	void TrailEffectDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateTrailEffectPipelineState(int blendMode);

	/* ==================== Animation ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateAnimationRootSignature();
	// シェーダーコンパイル
	void AnimationShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& geometryShader, ComPtr<IDxcBlob>& pixelShader);
	// デスクリプターステンシル
	void AnimationDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateAnimationPipelineState();

	/* ==================== RenderTexture ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateRenderTextureRootSignature();
	// シェーダーコンパイル
	void RenderTextureShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader);
	// デスクリプターステンシル
	void RenderTextureDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
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