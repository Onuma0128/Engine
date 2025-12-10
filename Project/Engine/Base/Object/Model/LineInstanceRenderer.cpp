#include "LineInstanceRenderer.h"

#include "DirectXEngine.h"
#include "SrvManager.h"
#include "TextureManager.h"
#include "CreateBufferResource.h"

#include "Camera.h"
#include "CameraManager.h"

std::unique_ptr<LineInstanceRenderer> LineInstanceRenderer::instance_ = nullptr;

LineInstanceRenderer* LineInstanceRenderer::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = std::make_unique<LineInstanceRenderer>();
    }
    return instance_.get();
}

void LineInstanceRenderer::Initialize(uint32_t capacity)
{
    base_ = std::make_unique<Line3dBase>();
    base_->Initialize();

    CreateLocalVB();
    CreateInstanceVB(capacity);
    CreateCB();
    CreateSB();
}

void LineInstanceRenderer::CreateLocalVB()
{
    LocalV local[2] = { {0.f}, {1.f} };
    vbLocal_ = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(local));
    void* dst = nullptr;
    vbLocal_->Map(0, nullptr, &dst);
    memcpy(dst, local, sizeof(local));
    vbLocal_->Unmap(0, nullptr);

    vbvLocal_.BufferLocation = vbLocal_->GetGPUVirtualAddress();
    vbvLocal_.StrideInBytes = sizeof(LocalV);
    vbvLocal_.SizeInBytes = sizeof(local);
}

void LineInstanceRenderer::CreateInstanceVB(uint32_t capacity)
{
    capacity_ = capacity;
    const UINT bytes = sizeof(Inst) * capacity_;
    vbInst_ = CreateBufferResource(DirectXEngine::GetDevice(), bytes);

    vbvInst_.BufferLocation = vbInst_->GetGPUVirtualAddress();
    vbvInst_.StrideInBytes = sizeof(Inst);
    vbvInst_.SizeInBytes = bytes;

    merged_.clear();
    merged_.reserve(capacity_);
    totalInstances_ = 0;
    mergedDirty_ = false;
}

void LineInstanceRenderer::CreateCB()
{
    cbVS_ = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(Matrix4x4));
    cbVS_->Map(0, nullptr, reinterpret_cast<void**>(&wvp_));
}

void LineInstanceRenderer::CreateSB()
{
    uint32_t maxInstance = 32768;

    sbPS_ = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(Material) * maxInstance);
    sbPS_->Map(0, nullptr, reinterpret_cast<void**>(&materialDatas_));
    for (uint32_t i = 0; i < maxInstance; ++i) {
        materialDatas_[i].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        materialDatas_[i].enableDraw = true;
        materialDatas_[i].enableLighting = true;
        materialDatas_[i].outlineMask = false;
        materialDatas_[i].outlineSceneColor = false;
        materialDatas_[i].uvTransform = Matrix4x4::Identity();
        materialDatas_[i].shininess = 20.0f;
        materialDatas_[i].environmentCoefficient = 0;
    }

    materialSrvIndex_ = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
    SrvManager::GetInstance()->CreateSRVforStructuredBuffer(
        materialSrvIndex_, sbPS_.Get(), maxInstance, sizeof(Material));
}

void LineInstanceRenderer::GrowIfNeeded(uint32_t needInstances)
{
    if (needInstances <= capacity_) return;

    // 倍々で拡張
    uint32_t newCap = capacity_ ? capacity_ * 2u : 1024u;
    while (newCap < needInstances) newCap *= 2u;

    // 既存を退避
    std::vector<Inst> oldMerged = std::move(merged_);

    CreateInstanceVB(newCap);

    // 戻す
    merged_ = std::move(oldMerged);
    totalInstances_ = static_cast<uint32_t>(merged_.size());

    // GPU の中身は次の Update() で全量コピーされるのでOK
    mergedDirty_ = true;
}

void LineInstanceRenderer::RegisterLine(Line3d* owner)
{
    if (!owner) return;
    if (entries_.find(owner) != entries_.end()) return;

    Entry e{};
    e.lineID = nextLineID_++;
    e.dirty = false;
    entries_.emplace(owner, std::move(e));
}

void LineInstanceRenderer::UnregisterLine(Line3d* owner)
{
    if (!owner) return;
    auto it = entries_.find(owner);
    if (it == entries_.end()) return;

    entries_.erase(it);
    // マージし直し
    mergedDirty_ = true;
}

uint32_t LineInstanceRenderer::GetLineID(Line3d* owner)
{
    return entries_[owner].lineID;
}

void LineInstanceRenderer::SetLineInstances(Line3d* owner, const std::vector<Vector3>& positions)
{
    if (!owner) return;
    // 未登録なら登録
    RegisterLine(owner);

    auto& e = entries_[owner];

    assert((positions.size() & 1u) == 0 && "positions は偶数個が必要です");

    const uint32_t segs = static_cast<uint32_t>(positions.size() / 2);
    e.instances.resize(segs);
    for (uint32_t i = 0; i < segs; ++i) {
        e.instances[i].startPos = positions[i * 2 + 0];
        e.instances[i].endPos = positions[i * 2 + 1];
        e.instances[i].lineIndex = e.lineID; // 全セグメント同じ lineID
    }
    e.dirty = true;
    mergedDirty_ = true;
}

void LineInstanceRenderer::SetMaterial(Line3d* owner, const Material& material)
{
    uint32_t id = entries_[owner].lineID;
    materialDatas_[id] = material;
}

void LineInstanceRenderer::Finalize(bool instanceDelete)
{
    entries_.clear();
    merged_.clear();
    nextLineID_ = 0;
    totalInstances_ = 0;
    mergedDirty_ = true;
    if (instanceDelete) { instance_ = nullptr; }
}

void LineInstanceRenderer::Update()
{
    // VP 更新
    *wvp_ = CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix();

    if (!mergedDirty_) return;

    // まず総インスタンス数を数える
    uint32_t total = 0;
    for (auto& [ptr, e] : entries_) {
        total += static_cast<uint32_t>(e.instances.size());
    }

    // 必要なら拡張
    GrowIfNeeded(total);

    // マージ
    merged_.clear();
    merged_.reserve(total);
    for (auto& [ptr, e] : entries_) {
        if (!e.instances.empty()) {
            merged_.insert(merged_.end(), e.instances.begin(), e.instances.end());
        }
        e.dirty = false;
    }
    totalInstances_ = total;

    // GPU 転送
    if (totalInstances_ > 0) {
        void* dst = nullptr;
        vbInst_->Map(0, nullptr, &dst);
        memcpy(dst, merged_.data(), sizeof(Inst) * totalInstances_);
        vbInst_->Unmap(0, nullptr);
    }

    mergedDirty_ = false;
}

void LineInstanceRenderer::Draws()
{
    if (totalInstances_ == 0) return;

    base_->DrawBase();

    D3D12_VERTEX_BUFFER_VIEW views[2] = { vbvLocal_, vbvInst_ };
    DirectXEngine::GetCommandList()->IASetVertexBuffers(0, 2, views);

    DirectXEngine::GetCommandList()->SetGraphicsRootConstantBufferView(0, cbVS_->GetGPUVirtualAddress());
    SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, materialSrvIndex_);

    DirectXEngine::GetCommandList()->DrawInstanced(2, totalInstances_, 0, 0);
}
