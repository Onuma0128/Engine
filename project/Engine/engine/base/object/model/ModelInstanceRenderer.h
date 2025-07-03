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

    /* ========================= Object3d ========================= */

    void Push(Object3d* obj);
    void Remove(Object3d* obj);

    /* ========================= Animation ========================= */

    void Push(Animation* animation);
    void Remove(Animation* animation);


    void Finalize() { 
        objBatches_.clear();
        animationBatches_.clear();
    }

    // 全ての描画
    void AllDraw();

private:

    /* ========================= Object3d ========================= */

    void ObjReserveBatch(Model* model, uint32_t maxInstance = 128);
    void ObjUpdate();

    /* ========================= Animation ========================= */

    void AnimationReserveBatch(Model* model, uint32_t maxInstance = 128);
    void AnimationUpdate();

    int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

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

};

