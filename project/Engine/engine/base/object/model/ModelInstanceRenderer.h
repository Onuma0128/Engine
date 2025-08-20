#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>
using Microsoft::WRL::ComPtr;

#include <unordered_map>
#include <vector>

#include "ModelStruct.h"

class WorldTransform;
class Object3d;
class Animation;
class Model;

class ModelInstanceRenderer
{
public:

    struct InstanceData   // VS側で読む | インスタンス分の情報
    {
        Matrix4x4 WVP;
        Matrix4x4 World;
        Matrix4x4 WorldInvT;
    };
    struct JointCount 
    {
        uint32_t jointCount;
    };

    void Initialize();

    /* ========================= Object3d ========================= */

    void Push(Object3d* obj);
    void Remove(Object3d* obj);

    /* ========================= Animation ========================= */

    void Push(Animation* animation);
    void Remove(Animation* animation);


    void Finalize() { 
        for (auto& obj : objBatches_) {
            obj.second.objects.clear();
            obj.second.count = 0;
        }
        for (auto& anima : animationBatches_) {
            anima.second.animations.clear();
            anima.second.count = 0;
        }
    }

    // 全ての描画
    void AllDraw();

    void AllDrawOutlineMask();

private:

    /* ========================= Object3d ========================= */

    void ObjReserveBatch(Object3d* object, uint32_t maxInstance = 128);
    void ObjUpdate();

    /* ========================= Animation ========================= */

    void AnimationReserveBatch(Animation* animation, uint32_t maxInstance = 128);
    void AnimationUpdate();

private:

    /* ========================= Object3d ========================= */

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

