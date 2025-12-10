#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>
using Microsoft::WRL::ComPtr;

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <memory>
#include <cassert>

#include "Line3dBase.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "ModelStruct.h"

class Line3d;

/// <summary>
/// ラインのインスタンス、描画を管理するクラス
/// </summary>
class LineInstanceRenderer
{
private:

    // シングルトンインスタンス
    static std::unique_ptr<LineInstanceRenderer> instance_;

public:

    LineInstanceRenderer() = default;
    ~LineInstanceRenderer() = default;
    LineInstanceRenderer(LineInstanceRenderer&) = delete;
    LineInstanceRenderer& operator=(LineInstanceRenderer&) = delete;

    // シングルトンインスタンスの取得
    static LineInstanceRenderer* GetInstance();

    // capacity は初期の線分(インスタンス)予約数
    void Initialize(uint32_t capacity = 2048);

	// ラインの登録/解除/ID取得
    void RegisterLine(Line3d* owner);
    void UnregisterLine(Line3d* owner);
    uint32_t GetLineID(Line3d* owner);

	// ラインのインスタンスデータ/マテリアル設定
    void SetLineInstances(Line3d* owner, const std::vector<Vector3>& positions);
    void SetMaterial(Line3d* owner, const Material& material);

    // 全破棄
    void Finalize(bool instanceDelete = false);

    // 毎フレーム（VP更新と必要ならGPUアップロード）
    void Update();

    // 描画
    void Draws();

private:
    struct LocalV { float t; }; // t=0/1 の2頂点

    // InputLayoutFactory::Line3dInputLayout と一致
    struct Inst {
        Vector3 startPos;
        Vector3 endPos;
        uint32_t lineIndex; // Shader 側で Transform/Material のインデックスに使える
    };

    struct Entry {
        uint32_t lineID = UINT32_MAX;
        std::vector<Inst> instances; // ownerの線分
        bool dirty = false;          // この owner が更新された
    };

	// 各種作成
    void CreateLocalVB();
    void CreateInstanceVB(uint32_t capacity);
    void GrowIfNeeded(uint32_t needInstances);
    void CreateCB();
    void CreateSB();

private:
    // PSO/RS/Topology セット用
    std::unique_ptr<Line3dBase> base_;

    // VB
    ComPtr<ID3D12Resource> vbLocal_;
    ComPtr<ID3D12Resource> vbInst_;
    D3D12_VERTEX_BUFFER_VIEW vbvLocal_{};
    D3D12_VERTEX_BUFFER_VIEW vbvInst_{};

    // VS用 CB（ViewProjection）
    ComPtr<ID3D12Resource> cbVS_;
    Matrix4x4* wvp_ = nullptr;
    // PS用 SB (Color)
    ComPtr<ID3D12Resource> sbPS_;
    Material* materialDatas_;               
    uint32_t materialSrvIndex_;            

    // 管理
    std::unordered_map<Line3d*, Entry> entries_;
    uint32_t nextLineID_ = 0;

    // マージ後の大きなインスタンス配列（CPU 側）
    std::vector<Inst> merged_;
    bool mergedDirty_ = false; // どれかが dirty なら true

    // 現在の総インスタンス数（=総線分数）
    uint32_t totalInstances_ = 0;
    uint32_t capacity_ = 0;
};

