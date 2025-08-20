#include "ParticleManager.h"

#include <numbers>
#include "imgui.h"

#include "DirectXEngine.h"
#include "PipelineState.h"
#include "SrvManager.h"
#include "Line3dBase.h"
#include "LightManager.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "ModelManager.h"

#include "Camera.h"
#include "CreateBufferResource.h"
#include "ParticleEmitter.h"
#include "ParticleEditor.h"

ParticleManager* ParticleManager::instance_ = nullptr;

ParticleManager* ParticleManager::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new ParticleManager;
    }
    return instance_;
}

void ParticleManager::Initialize(DirectXEngine* dxEngine)
{
    dxEngine_ = dxEngine;
    srvManager_ = SrvManager::GetInstance();

    rootSignature_ = dxEngine_->GetPipelineState()->GetRootSignature(PipelineType::Particle).Get();
    for (int i = 0; i < static_cast<int>(pipelineStates_.size()); ++i) {
        pipelineStates_[i] = dxEngine_->GetPipelineState()->GetPipelineState(
            PipelineType::Particle, PostEffectType::None, static_cast<BlendMode>(i)
        ).Get();
    }

    CreateVertexResource();
    CreateVertexData();
    CreateIndexResource();
    CreateIndexData();
}

void ParticleManager::Update()
{
#ifdef _DEBUG
    // editorのUpdate
    ParticleEditorUpdate();
#endif // _DEBUG

    // 各パーティクルグループに対して処理
    for (auto it = particleGroups_.begin(); it != particleGroups_.end();) {
        auto& group = it->second;

        size_t emitterSize = group.emitters.size();
        group.emitters.erase(
            std::remove_if(group.emitters.begin(), group.emitters.end(),
                [](const std::weak_ptr<ParticleEmitter>& w) {
                    return w.expired(); }),
                    group.emitters.end());

        // Emitterが消えているならCopyのNameを詰める
        uint32_t emitCount = 0;
        for (auto& g_emitter : group.emitters) {
            if (emitterSize == group.emitters.size()) { break; }
            if (auto emitter = g_emitter.lock()) {
                std::string name = emitter->GetName();
                if (emitCount == 0) {
                    emitter->SetCopyName(name);
                } else {
                    emitter->SetCopyName(name + std::to_string(static_cast<uint32_t>(emitCount)));
                }
            }
            ++emitCount;
        }

        std::vector<ParticleForGPU> staging;
        uint32_t numInstance = 0;
        if (!group.emitters.empty()) {
            // 新しいパーティクルを追加する
            for (auto& g_emitter : group.emitters) {
                if (auto emitter = g_emitter.lock()) {
                    emitter->SetEmitter(group.editor->GetBaseEmitter());
#ifdef _DEBUG
                    emitter->SetDrawLine(drawEmitter_);
#endif // _DEBUG
                    emitter->Update();
                }
            }
            Emit(it->first);

            EnsureInstanceCapacity(group, static_cast<uint32_t>(group.particles.size()));

            // 各パーティクルを更新
            for (auto p_it = group.particles.begin(); p_it != group.particles.end();) {
                if (p_it->lifeTime <= p_it->currentTime) {
                    p_it = group.particles.erase(p_it);
                    continue;
                }

                // パーティクルのビルボード化
                Matrix4x4 billboardMatrix = Matrix4x4::Identity();
                if (group.editor->GetBillboard()) {
                    billboardMatrix = Matrix4x4::Rotate(p_it->transform.rotation) * CameraManager::GetInstance()->GetActiveCamera()->GetWorldMatrix();
                } else {
                    billboardMatrix = Matrix4x4::Rotate(p_it->transform.rotation);
                }
                billboardMatrix.m[3][0] = 0.0f;
                billboardMatrix.m[3][1] = 0.0f;
                billboardMatrix.m[3][2] = 0.0f;
                billboardMatrix.m[3][3] = 1.0f;
                Matrix4x4 worldMatrix = Matrix4x4::Scale(p_it->transform.scale) * billboardMatrix * Matrix4x4::Translate(p_it->transform.translation);
                Matrix4x4 worldViewMatrix = worldMatrix * CameraManager::GetInstance()->GetActiveCamera()->GetViewMatrix();
                Matrix4x4 worldViewProjectionMatrix = worldViewMatrix * CameraManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix();

                // パーティクルの更新
                for (auto& g_emitter : group.emitters) {
                    if (auto emitter = g_emitter.lock()) {
                        if (emitter->GetCopyName() == p_it->emitterName) {
                            emitter->UpdateParticle(p_it);
                        }
                    }
                }

                // uvFlip
                group.materialData_->isUVFlipX = group.editor->GetBaseEmitter().isUvFlipX;
                group.materialData_->isUVFlipY = group.editor->GetBaseEmitter().isUvFlipY;
                // uvTranslate
                group.materialData_->uvTransform = Matrix4x4::Translate(p_it->uvTranslate);

                float alpha = 1.0f - (p_it->currentTime / p_it->lifeTime);
                if (numInstance < group.maxInstance) {
                    staging.push_back({});
                    staging[numInstance].WVP = worldViewProjectionMatrix;
                    staging[numInstance].World = worldViewMatrix;
                    staging[numInstance].color = p_it->color;
                    staging[numInstance].color.w = alpha;
                    ++numInstance;
                }
                ++p_it;
            }

            group.instanceCount = numInstance;
            const uint32_t bytes = UINT(staging.size() * sizeof(ParticleForGPU));
            std::memcpy(group.instancingData, staging.data(), bytes);
            ++it;

        } else {
            ++it;
        }
    }
}

void ParticleManager::Draw()
{
    auto commandList = dxEngine_->GetCommandList();

    for (const auto& [name, group] : particleGroups_) {

        /*==================== カウントが0なら描画しない ====================*/
        if (group.instanceCount == 0) { continue; }

        /*==================== パイプラインの設定 ====================*/
        commandList->SetGraphicsRootSignature(rootSignature_.Get());
        commandList->SetPipelineState(pipelineStates_[particleGroups_[name].editor->GetBlendMode()].Get());
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        /*==================== パーティクルの描画 ====================*/
        uint32_t textIndex = group.textureIndex;
        UINT drawIndex = 6;
        D3D12_VERTEX_BUFFER_VIEW vertex = vertexPlane_.vertexBufferView;
        D3D12_INDEX_BUFFER_VIEW index = indexPlane_.indexBufferView;
        if (particleGroups_[name].editor->GetModel() == 1) {
            vertex = vertexRing_.vertexBufferView;
            index = indexRing_.indexBufferView;
            drawIndex = 192;
        }
        commandList->IASetVertexBuffers(0, 1, &vertex);
        commandList->IASetIndexBuffer(&index);
        commandList->SetGraphicsRootConstantBufferView(0, group.materialResource_->GetGPUVirtualAddress());
        commandList->SetGraphicsRootConstantBufferView(1, group.instancingResource->GetGPUVirtualAddress());
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, textIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(3, group.srvIndex);

        // 各パーティクルグループのインスタンスを描画
        commandList->DrawIndexedInstanced(drawIndex, group.instanceCount, 0, 0, 0);
    }
}

void ParticleManager::Finalize()
{
    delete instance_;
    instance_ = nullptr;
}

void ParticleManager::Clear()
{
    for (auto& group : particleGroups_) {
        group.second.particles.clear();
        group.second.emitters.clear();
        group.second.instanceCount = 0;
    }
}

void ParticleManager::CreateParticleGroup(std::shared_ptr<ParticleEmitter> emitter)
{
    std::string name = emitter->GetName();

    auto it = particleGroups_.find(name);
    if (it != particleGroups_.end()) {

        // 既存グループのエミッターを複製
        std::string copyName = emitter->GetName();
        copyName = copyName + std::to_string(static_cast<uint32_t>(it->second.emitters.size()));
        emitter->SetCopyName(copyName);
        it->second.emitters.push_back(emitter);

        return;
    }

    ParticleGroup group;

    // パーティクルグループのマテリアル用のリソース,データを作成
    CreateMatrialResource(group);

    // パーティクルグループのインスタンス数とリソースを初期化
    group.maxInstance = kNumMaxInstance;
    group.instanceCount = 0;
    group.instancingData = nullptr;

    // インスタンスデータ用のバッファリソースを作成
    group.instancingResource = CreateBufferResource(
        dxEngine_->GetDevice(),
        sizeof(ParticleForGPU) * kNumMaxInstance);

    // リソースをマッピングして、インスタンスデータを初期化
    group.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&group.instancingData));
    for (uint32_t i = 0; i < kNumMaxInstance; ++i) {
        group.instancingData[i].WVP = Matrix4x4::Identity();
        group.instancingData[i].World = Matrix4x4::Identity();
        group.instancingData[i].color = Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
    }
    group.emitters.push_back(emitter);
    group.editor = std::make_unique<ParticleEditor>();
    group.editor->Initialize(name);

    group.textureFilePath = group.editor->GetTexture();
    group.textureIndex = TextureManager::GetInstance()->GetSrvIndex(group.textureFilePath);

    group.srvIndex = srvManager_->Allocate() + TextureManager::kSRVIndexTop;

    srvManager_->CreateSRVforStructuredBuffer(
        group.srvIndex,
        group.instancingResource.Get(),
        kNumMaxInstance,
        sizeof(ParticleManager::ParticleForGPU)
    );

    // 新しいパーティクルグループを particleGroups_ マップに追加
    particleGroups_[name] = std::move(group);
}

void ParticleManager::Emit(const std::string name)
{
    // 新しいパーティクルを追加する
    for (auto& g_emitter : particleGroups_[name].emitters) {
        if (auto emitter = g_emitter.lock()) {
            emitter->CreateParticles(particleGroups_[name]);
        }
    }
}

void ParticleManager::ParticleEditorUpdate()
{
    // 選択できるアイテム
    std::vector<std::string> items;
    // 選択中インデックス
    static int current = 0;
    // パーティクルのEditor処理
    for (auto it = particleGroups_.begin(); it != particleGroups_.end();) {
        std::string name = it->first;
        items.push_back(name);
        ++it;
    }
    if (items.size() < current) { current = 0; }
    // ImGuiの描画
    ImGui::Begin("ParicleEditor");
    if (ImGui::BeginCombo("EditorFile", items[current].c_str())) {
        for (int i = 0; i < items.size(); ++i) {
            const bool is_selected = (current == i);
            if (ImGui::Selectable(items[i].c_str(), is_selected)) {
                current = i;                      // 切り替え
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();     // 選択項目にフォーカス
            }
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();                      
    ImGui::Checkbox("DrawEmitter", &drawEmitter_);
    ImGui::Text("%d", particleGroups_[items[current]].particles.size());

    // 選択中のnameを取得、ImGuiを描画
    std::string selectedName = items[current];
    particleGroups_[selectedName].editor->Update();
    if (particleGroups_[selectedName].editor->GetTexture() != "") {
        particleGroups_[selectedName].textureFilePath = particleGroups_[selectedName].editor->GetTexture();
        particleGroups_[selectedName].textureIndex = TextureManager::GetInstance()->GetSrvIndex(particleGroups_[selectedName].textureFilePath);
    }

    ImGui::End();
}

void ParticleManager::CreateVertexData()
{
    /* ============================== Plane ============================== */

    vertexPlane_.vertexData[0].position = { -1.0f,1.0f,0.0f,1.0f };
    vertexPlane_.vertexData[1].position = { 1.0f,1.0f,0.0f,1.0f };
    vertexPlane_.vertexData[2].position = { -1.0f,-1.0f,0.0f,1.0f };
    vertexPlane_.vertexData[3].position = { 1.0f,-1.0f,0.0f,1.0f };

    vertexPlane_.vertexData[0].texcoord = { 0.0f,0.0f };
    vertexPlane_.vertexData[1].texcoord = { 1.0f,0.0f };
    vertexPlane_.vertexData[2].texcoord = { 0.0f,1.0f };
    vertexPlane_.vertexData[3].texcoord = { 1.0f,1.0f };

    /* ============================== RIng ============================== */

    const float kRingDivide = 32;
    const float kOuterRadius = 1.0f;
    const float kInnerRadius = 0.2f;

    for (uint32_t i = 0; i <= kRingDivide; ++i) {
        float t = static_cast<float>(i) / kRingDivide;   // 0.0 … 1.0
        float rad = t * 2.0f * std::numbers::pi_v<float>;
        float s = std::sin(rad);
        float c = std::cos(rad);

        uint32_t counter = i * 2;
        // 外周
        vertexRing_.vertexData[counter].position = {-s * kOuterRadius,  c * kOuterRadius, 0.0f, 1.0f};
        vertexRing_.vertexData[counter].texcoord = { t, 0.0f };
        // 内周
        vertexRing_.vertexData[counter + 1].position = { -s * kInnerRadius,  c * kInnerRadius, 0.0f, 1.0f };
        vertexRing_.vertexData[counter + 1].texcoord = { t, 1.0f };
    }

}

void ParticleManager::CreateVertexResource()
{
    // 実際に頂点リソースを作る
    vertexPlane_.vertexResource = CreateBufferResource(dxEngine_->GetDevice(), sizeof(VertexData) * 4);
    vertexPlane_.vertexBufferView.BufferLocation = vertexPlane_.vertexResource->GetGPUVirtualAddress();
    vertexPlane_.vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * 4);
    vertexPlane_.vertexBufferView.StrideInBytes = sizeof(VertexData);

    vertexPlane_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexPlane_.vertexData));

    // 実際に頂点リソースを作る
    vertexRing_.vertexResource = CreateBufferResource(dxEngine_->GetDevice(), sizeof(VertexData) * 66);
    vertexRing_.vertexBufferView.BufferLocation = vertexRing_.vertexResource->GetGPUVirtualAddress();
    vertexRing_.vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * 66);
    vertexRing_.vertexBufferView.StrideInBytes = sizeof(VertexData);

    vertexRing_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexRing_.vertexData));
}

void ParticleManager::CreateIndexData() 
{
    /* ============================== Plane ============================== */

    indexPlane_.indexData[0] = 0; indexPlane_.indexData[1] = 1; indexPlane_.indexData[2] = 2;
    indexPlane_.indexData[3] = 1; indexPlane_.indexData[4] = 3; indexPlane_.indexData[5] = 2;

    /* ============================== Ring ============================== */

    const float kRingDivide = 32;

    for (uint32_t i = 0; i < kRingDivide; ++i) {
        uint32_t v0 = 2 * i;     // 外周  (current)
        uint32_t v1 = 2 * i + 1; // 内周  (current)
        uint32_t v2 = 2 * (i + 1);     // 外周  (next)
        uint32_t v3 = 2 * (i + 1) + 1; // 内周  (next)

        uint32_t counter = i * 6;

        // 三角形 1
        indexRing_.indexData[counter] = v0;
        indexRing_.indexData[counter + 1] = v1;
        indexRing_.indexData[counter + 2] = v2;

        // 三角形 2
        indexRing_.indexData[counter + 3] = v2;
        indexRing_.indexData[counter + 4] = v1;
        indexRing_.indexData[counter + 5] = v3;
    }
}

void ParticleManager::CreateIndexResource()
{
    indexPlane_.indexResource = CreateBufferResource(dxEngine_->GetDevice(), sizeof(uint32_t) * 6);
    indexPlane_.indexBufferView.BufferLocation = indexPlane_.indexResource->GetGPUVirtualAddress();
    indexPlane_.indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
    indexPlane_.indexBufferView.Format = DXGI_FORMAT_R32_UINT;

    indexPlane_.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexPlane_.indexData));


    indexRing_.indexResource = CreateBufferResource(dxEngine_->GetDevice(), sizeof(uint32_t) * 6 * 32);
    indexRing_.indexBufferView.BufferLocation = indexRing_.indexResource->GetGPUVirtualAddress();
    indexRing_.indexBufferView.SizeInBytes = sizeof(uint32_t) * 6 * 32;
    indexRing_.indexBufferView.Format = DXGI_FORMAT_R32_UINT;

    indexRing_.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexRing_.indexData));
}

void ParticleManager::CreateMatrialResource(ParticleGroup& group)
{
    // マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
    group.materialResource_ = CreateBufferResource(dxEngine_->GetDevice(), sizeof(Material));
    // 書き込むためのアドレスを取得
    group.materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&group.materialData_));
    // 今回は白を書き込んでいく
    group.materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    group.materialData_->enableLighting = false;
    group.materialData_->uvTransform = Matrix4x4::Identity();
}

void ParticleManager::EnsureInstanceCapacity(ParticleGroup& group, uint32_t required)
{
    if (required <= group.maxInstance) return;

    // 成長戦略: 2倍ずつ（必要数を下回らない最小の2冪）
    uint32_t newCap = std::max(1u, group.maxInstance);
    while (newCap < required) { newCap *= 2; }

    // 新しいアップロードバッファを確保
    group.instancingResource = CreateBufferResource(
        dxEngine_->GetDevice(),
        sizeof(ParticleForGPU) * newCap
    );

    // 再マップ（ポインタ更新）
    group.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&group.instancingData));

    // 必要なら初期化（省略可）
    for (uint32_t i = 0; i < newCap; ++i) {
        group.instancingData[i].WVP = Matrix4x4::Identity();
        group.instancingData[i].World = Matrix4x4::Identity();
        group.instancingData[i].color = Vector4{ 1,1,1,1 };
    }

    group.maxInstance = newCap;

    // SRV を同じインデックスで作り直して差し替え
    srvManager_->CreateSRVforStructuredBuffer(
        group.srvIndex,
        group.instancingResource.Get(),
        newCap,
        sizeof(ParticleForGPU)
    );
}

