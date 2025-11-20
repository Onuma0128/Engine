#pragma once
#include "wrl.h"
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <stdint.h>

#include "PostEffectType.h"
#include "Vector4.h"

using Microsoft::WRL::ComPtr;

class RenderTexture
{
public:

	// RenderTextureのResourceを作成
	static ComPtr<ID3D12Resource> CreateResource(
		ComPtr<ID3D12Device> device,
		uint32_t width,
		uint32_t height,
		DXGI_FORMAT format,
		const Vector4& clearColor
	);
	// RenderTextureのSRVを作成
	uint32_t GetRenderTextureSRVIndex() const { return renderTextureSRVIndex_; }
	//  FinalSrvIndexの取得と設定
	uint32_t GetFinalSrvIndex() const { return finalSrvIndex_; }
	void SetFinalSrvIndex(uint32_t index) { finalSrvIndex_ = index; }

	// 深度ステンシルバッファのResourceとDSVハンドルを取得
	ID3D12Resource* GetDSVResource()const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle() const;
	const uint32_t GetDSVHandleIndex()const { return depthIndex_; }

	// 初期化
	void Initialize();

	// BarrierのStartとEnd
	void StartBarrier();
	void EndBarrier();

	// 描画前の処理と描画
	void PreDraw();
	void Draw();

private:

	// レンダーテクスチャを生成する
	ComPtr<ID3D12Resource> renderTextureResource_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE renderTextureHandle_{};
	uint32_t renderTextureSRVIndex_;
	uint32_t finalSrvIndex_;

	// 深度バッファの生成
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
	uint32_t depthIndex_ = 0;

	// TransitionBarrierの生成
	D3D12_RESOURCE_BARRIER barrier_{};

	// ルートシグネチャ,パイプラインステート
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

};
