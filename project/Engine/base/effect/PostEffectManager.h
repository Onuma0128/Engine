#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"

#include <stdint.h>
#include <vector>
#include <unordered_map>

#include "PostEffectType.h"

using Microsoft::WRL::ComPtr;

class DirectXEngine;
class PipelineState;

class PostEffectManager
{
public:

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize();

    /// <summary>
    /// 初期化
    /// </summary>
	void Initialize(DirectXEngine* dxEngine);

    /// <summary>
    /// ポストエフェクトを作成する
    /// </summary>
    /// <param name="type"></作成したいTypeを入れる>
    void CreatePostEffect(PostEffectType type);

	// アウトラインマスクパスの開始と終了
    void BeginOutlineMaskPass();
    void EndOutlineMaskPass();

	// ポストエフェクトのコマンド処理
    void PostEffectCommand(PostEffectType type);
    // ポストエフェクト単体の描画
    uint32_t DrawEffect(PostEffectType type, uint32_t inputSRVIndex);
    // ポストエフェクト全ての描画
    void RenderTextureDraws(uint32_t inputSRVIndex);
    // 最後のSRVIndexを保管
    uint32_t GetFinalSRVIndex()const { return finalSRVIndex_; }

    // ポストエフェクト全てのポインタ
    // GrayscaleData
    GrayscaleData* GetGrayscaleData() { return grayscaleData_; }
    // VignetteData
    VignetteData* GetVignetteData() { return vignetteData_; }

private:

    /// <summary>
    /// リソースの初期化
    /// </summary>
    void ResourceInitialize();

    /// <summary>
    /// アウトラインマスクリソースの作成
    /// </summary>
    void CreateOutLineMaskResource();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

private:

    struct PostEffectPass {
        ComPtr<ID3D12Resource> renderTexture;
        uint32_t srvIndex = 0;
        uint32_t rtvIndex = 0;
        uint32_t depthSrvIndex = 0;
    };
    struct OutLineMaskPass {
        ComPtr<ID3D12Resource> outlineMask;
        ComPtr<ID3D12Resource> objectID;
        uint32_t rtvIndex = 0;
        uint32_t rtvIndexID = 0;
        uint32_t depthDsvIndex = 0;
        uint32_t srvIndex = 0;
        uint32_t srvIndexID = 0;
    };


    DirectXEngine* dxEngine_ = nullptr;
    PipelineState* pipeline_ = nullptr;

    std::vector<PostEffectType> enabledEffects_;
    std::unordered_map<PostEffectType, PostEffectPass> passes_;
    std::unordered_map<PostEffectType, ComPtr<ID3D12PipelineState>> pipelineStates_;
    std::unordered_map<PostEffectType, ComPtr<ID3D12RootSignature>> rootSignatures_;

    uint32_t finalSRVIndex_;

    // GrayscaleData
    ComPtr<ID3D12Resource> grayscaleResource_ = nullptr;
    GrayscaleData* grayscaleData_;
    // VignetteData
    ComPtr<ID3D12Resource> vignetteResource_ = nullptr;
    VignetteData* vignetteData_;
    // OutlineData
    ComPtr<ID3D12Resource> outlineResource_ = nullptr;
    OutlineData* outlineData_;
    // OutlineMask
    OutLineMaskPass maskPass_;

};

