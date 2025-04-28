#pragma once
#include <Windows.h>
#include "wrl.h"
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")

using Microsoft::WRL::ComPtr;

enum class BlendMode {
	// ブレンド無し
	kBlendModeNone,

	// 通常αブレンド
	kBlendModeNormal,

	// 加算ブレンド
	kBlendModeAdd,

	// 減算ブレンド
	kBlendModeSubtract,

	// 乗算ブレンド
	kBlendModeMultily
};

class PipelineState
{
public:

	void Initialize(
		ComPtr<ID3D12Device>& device, ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler, ComPtr<IDxcIncludeHandler> includeHandler);

	/* ==================== Object3d ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateObject3dRootSignature();
	// インプットレイアウト
	void Object3dInputLayout(D3D12_INPUT_ELEMENT_DESC* inputElementDescs, D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc);
	// ラスタライザ
	void RasterizerState(D3D12_RASTERIZER_DESC& rasterizerDesc, bool enableCulling);
	// シェーダーコンパイル
	void Object3dShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& geometryShader, ComPtr<IDxcBlob>& pixelShader);
	// デスクリプターステンシル
	void Object3dDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateObject3dPipelineState();

	/* ==================== Sprite ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateSpriteRootSignature();
	// インプットレイアウト
	void SpriteInputLayout(D3D12_INPUT_ELEMENT_DESC* inputElementDescs, D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc);
	// デスクリプターステンシル
	void SpriteDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateSpritePipelineState();

	/* ==================== Line3d ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateLine3dRootSignature();
	// ラスタライザ
	void Line3dRasterizerState(D3D12_RASTERIZER_DESC& rasterizerDesc);
	// インプットレイアウト
	void Line3dInputLayout(D3D12_INPUT_ELEMENT_DESC* inputElementDescs, D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc);
	// シェーダーコンパイル
	void Line3dShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader);
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateLine3dPipelineState();

	/* ==================== Particle ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateParticleRootSignature();
	// インプットレイアウト
	void ParticleInputLayout(D3D12_INPUT_ELEMENT_DESC* inputElementDescs, D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc);
	// ブレンド
	void BlendState(D3D12_BLEND_DESC& blendDesc, int blendMode);
	// シェーダーコンパイル
	void ParticleShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader);
	// デスクリプターステンシル
	void ParticleDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateParticlePipelineState(int blendMode);

	/* ==================== TrailEffect ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateTrailEffectRootSignature();
	// インプットレイアウト
	void TrailEffectInputLayout(D3D12_INPUT_ELEMENT_DESC* inputElementDescs, D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc);
	// ラスタライザ
	void TrailEffectRasterizerState(D3D12_RASTERIZER_DESC& rasterizerDesc);
	// シェーダーコンパイル
	void TrailEffectShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader);
	// デスクリプターステンシル
	void TrailEffectDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateTrailEffectPipelineState(int blendMode);

	/* ==================== Animation ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateAnimationRootSignature();
	// インプットレイアウト
	void AnimationInputLayout(D3D12_INPUT_ELEMENT_DESC* inputElementDescs, D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc);
	// ラスタライザ
	void AnimationRasterizerState(D3D12_RASTERIZER_DESC& rasterizerDesc);
	// シェーダーコンパイル
	void AnimationShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& geometryShader, ComPtr<IDxcBlob>& pixelShader);
	// デスクリプターステンシル
	void AnimationDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateAnimationPipelineState();

	/* ==================== RenderTexture ==================== */

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateRenderTextureRootSignature();
	// インプットレイアウト
	void RenderTextureInputLayout(D3D12_INPUT_ELEMENT_DESC* inputElementDescs, D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc);
	// ラスタライザ
	void RenderTextureRasterizerState(D3D12_RASTERIZER_DESC& rasterizerDesc);
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