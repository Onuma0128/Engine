#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>
using Microsoft::WRL::ComPtr;

#include <unordered_map>
#include <vector>

// ===== 追加：LateDraw 制御用 =====
#include <string>
#include <unordered_set>
#include <algorithm>

#include "ModelStruct.h"

class WorldTransform;
class Object3d;
class Animation;
class Model;

/// <summary>
/// モデルのインスタンスを管理するクラス
/// </summary>
class ModelInstanceRenderer
{
private:

    // シングルトンインスタンス
    static std::unique_ptr<ModelInstanceRenderer> instance_;

public:

    ModelInstanceRenderer() = default;
    ~ModelInstanceRenderer() = default;
    ModelInstanceRenderer(ModelInstanceRenderer&) = delete;
    ModelInstanceRenderer& operator=(ModelInstanceRenderer&) = delete;

    // シングルトンインスタンスの取得
    static ModelInstanceRenderer* GetInstance();

    // インスタンスデータ構造体
    struct InstanceData
    {
        Matrix4x4 WVP;
        Matrix4x4 World;
        Matrix4x4 WorldInvT;
    };
    // ジョイント数構造体
    struct JointCount
    {
        uint32_t jointCount;
        int padding[3];
    };

    //初期化
    void Initialize();

    /* ========================= Object3d ========================= */

    // インスタンス登録・削除
    void Push(Object3d* obj);
    void Remove(Object3d* obj);

    /* ========================= Animation ========================= */

    // インスタンス登録・削除
    void Push(Animation* animation);
    void Remove(Animation* animation);

    // 終了処理
    void Finalize() {
        for (auto& obj : objBatches_) {
            obj.second.objects.clear();
            obj.second.count = 0;
        }
        for (auto& anima : animationBatches_) {
            anima.second.animations.clear();
            anima.second.count = 0;
        }

        // LateDraw
        lateDrawModelNames_.clear();
        objDrawOrder_.clear();
        objLateDrawOrder_.clear();
        animDrawOrder_.clear();
        animLateDrawOrder_.clear();

        instance_ = nullptr;
    }

    // 全てのシャドウマップ深度パス描画
    void AllDrawShadowDepth();
    // 全てのアウトラインマスク描画
    void AllDrawOutlineMask();
    // 全ての描画
    void AllDraw();

    /* ========================= LateDraw ========================= */

    // 指定したモデル名を「最後に描画」する対象に追加
    void AddLateDrawModelName(const std::string& modelName);
    // 指定したモデル名を「最後に描画」対象から削除
    void RemoveLateDrawModelName(const std::string& modelName);
    // 「最後に描画」対象を全削除
    void ClearLateDrawModelNames();

private:

    /* ========================= Object3d ========================= */

    // インスタンスバッチ確保・更新
    void ObjReserveBatch(Object3d* object, uint32_t maxInstance = 128);
    void ObjUpdate();

    /* ========================= Animation ========================= */

    // インスタンスバッチ確保・更新
    void AnimationReserveBatch(Animation* animation, uint32_t maxInstance = 128);
    void AnimationUpdate();

private:

    /* ========================= LateDraw ========================= */

    // どのモデルが Late 対象か（名前で指定）
    std::unordered_set<std::string> lateDrawModelNames_;

    // 描画順（unordered_map を回さず、必ずこの順で描画する）
    std::vector<Model*> objDrawOrder_;
    std::vector<Model*> objLateDrawOrder_;
    std::vector<Model*> animDrawOrder_;
    std::vector<Model*> animLateDrawOrder_;

    // Model が Late 対象か
    bool IsLateDrawModel(Model* model) const;
    // 既に確保済みのモデルを「末尾（Late）」へ移動
    void MoveObjModelToLate(Model* model);
    void MoveAnimModelToLate(Model* model);

private:

    ComPtr<ID3D12RootSignature> objShadowMapRootSignature_; // ルートシグネチャ
    ComPtr<ID3D12PipelineState> objShadowMapPipelineState_; // パイプラインステート
    ComPtr<ID3D12RootSignature> animationShadowMapRootSignature_; // ルートシグネチャ
    ComPtr<ID3D12PipelineState> animationShadowMapPipelineState_; // パイプラインステート

    struct LightData {
        Matrix4x4 lightVP;
    };
    ComPtr<ID3D12Resource> lightVpBuffer_;
    LightData* lightData_ = nullptr;

    /* ========================= Object3d ========================= */

    // インスタンスバッチ構造体
    struct ObjectBatch
    {
        Model* model;                               // キー
        std::vector<Object3d*> objects;             // Objects
        uint32_t maxInstance;                       // 初期確保上限 (128)
        uint32_t count;                             // 今フレーム登録数

        ComPtr<ID3D12Resource> worldMatrixBuffer;   // StructuredBuffer
        InstanceData* instanceData;                 // Mapしたポインタ
        uint32_t instSrvIndex;                      // SRV番地 (worldMatrix)

        ComPtr<ID3D12Resource> materialBuffer;      // StructuredBuffer
        Material* materialData;                     // Mapしたポインタ
        uint32_t materialSrvIndex;                  // SRV番地 (material)
    };
    std::unordered_map<Model*, ObjectBatch> objBatches_;

    ComPtr<ID3D12RootSignature> objMaskRootSignature_; // ルートシグネチャ
    ComPtr<ID3D12PipelineState> objMaskPipelineState_; // パイプラインステート

    /* ========================= Animation ========================= */

    // インスタンスバッチ構造体
    struct AnimationBatch
    {
        Model* model;                               // キー
        std::vector<Animation*> animations;         // Animations
        uint32_t maxInstance;                       // 初期確保上限 (128)
        uint32_t count;                             // 今フレーム登録数

        ComPtr<ID3D12Resource> worldMatrixBuffer;   // StructuredBuffer
        InstanceData* instanceData;                 // Mapしたポインタ
        uint32_t instSrvIndex;                      // SRV番地 (worldMatrix)

        ComPtr<ID3D12Resource> materialBuffer;      // StructuredBuffer
        Material* materialData;                     // Mapしたポインタ
        uint32_t materialSrvIndex;                  // SRV番地 (material)

        ComPtr<ID3D12Resource> jointBuffer;         // ConstantBuffer
        JointCount* jointData;                       // Mapしたポインタ

        ComPtr<ID3D12Resource> paletteBuffer;       // StructuredBuffer
        WellForGPU* paletteData;                    // Mapしたポインタ
        uint32_t paletteSrvIndex;                   // SRV番地 (matrixPalette)
    };
    std::unordered_map<Model*, AnimationBatch> animationBatches_;

    ComPtr<ID3D12RootSignature> animaMaskRootSignature_; // ルートシグネチャ
    ComPtr<ID3D12PipelineState> animaMaskPipelineState_; // パイプラインステート

};

