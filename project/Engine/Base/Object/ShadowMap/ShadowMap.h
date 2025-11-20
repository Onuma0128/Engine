#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"

#include <stdint.h>

using Microsoft::WRL::ComPtr;

/// <summary>
/// シャドウマップクラス
/// </summary>
class ShadowMap
{
public:

    /// <summary>
    /// シャドウマップの作成
    /// </summary>
    /// <param name="size"></param>
    void CreateShadowMap(uint32_t size = 2048);

    /// <summary>
    /// シャドウマップの描画開始
    /// </summary>
    void BeginShadowMapPass();

    /// <summary>
    /// シャドウマップの描画終了
    /// </summary>
    void EndShadowMapPass();

    /// <summary>
    /// シャドウマップのSRVインデックスを取得
    /// </summary>
    /// <returns></returns>
    uint32_t GetShadowMapSrvIndex() const { return shadowSrvIndex_; }

    /// <summary>
    /// リソースの状態を遷移させる
    /// </summary>
    /// <param name="cmd"></param>
    /// <param name="newState"></param>
    void TransitionIfNeeded(ID3D12GraphicsCommandList* cmd, D3D12_RESOURCE_STATES newState);

private:

    // シャドウマップ用テクスチャ
	ComPtr<ID3D12Resource> shadowMapTexture_;

	// シャドウマップ用DSVハンドル
    D3D12_CPU_DESCRIPTOR_HANDLE shadowDSV_{};

	// シャドウマップ用SRVインデックス
    uint32_t shadowSrvIndex_ = 0;

    D3D12_RESOURCE_STATES shadowState_ = D3D12_RESOURCE_STATE_COMMON;

    D3D12_VIEWPORT shadowViewport_{};
    D3D12_RECT     shadowScissor_{};
    uint32_t shadowSize_ = 0;

};
