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
        Vector4   color;
    };

    void Push(Object3d* obj);
    void Push(Animation* anima);
    void Remove(Object3d* obj);
    void Remove(Animation* anima);
    void Finalize() { 
        objBatches_.clear();
        animaBatches_.clear();
    }

    // 全ての描画
    void AllDraw();

private:

    void ObjReserveBatch(Model* model, uint32_t maxInstance = 128);
    void AnimaReserveBatch(Model* model, uint32_t maxInstance = 128);

    void ObjUpdate();
    void AnimaUpdate();

private:

    struct ObjectBatch
    {
        Model* model;                         // キー
        std::vector<Object3d*> objects;       // Objects
        uint32_t maxInstance;                 // 初期確保上限 (128)
        uint32_t count;                       // 今フレーム登録数

        ComPtr<ID3D12Resource> gpuBuffer;     // StructuredBuffer
        InstanceData* cpuPtr;                 // Mapしたポインタ
        uint32_t srvIndex;                    // SRV番地 (descriptor heap 上)
    };
    std::unordered_map<Model*, ObjectBatch> objBatches_;


    struct AnimationBatch
    {
        Model* model;                         // キー
        std::vector<Animation*> animations;   // Animations
        uint32_t maxInstance;                 // 初期確保上限 (128)
        uint32_t count;                       // 今フレーム登録数
        ComPtr<ID3D12Resource> gpuBuffer;     // StructuredBuffer
        InstanceData* cpuPtr;                 // Mapしたポインタ
        uint32_t srvIndex;                    // SRV番地 (descriptor heap 上)
    };
    std::unordered_map<Model*, AnimationBatch> animaBatches_;

};

