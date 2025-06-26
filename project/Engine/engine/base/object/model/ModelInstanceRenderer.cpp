#include "ModelInstanceRenderer.h"

#include "DirectXEngine.h"
#include "SrvManager.h"
#include "TextureManager.h"

#include "Object3d.h"
#include "Animation.h"
#include "Model.h"
#include "WorldTransform.h"

#include "CreateBufferResource.h"

void ModelInstanceRenderer::ObjReserveBatch(Model* model, uint32_t maxInstance)
{
    // 二重確保ガード
    if (objBatches_.contains(model)) return;

    ObjectBatch batch{};
    batch.model = model;
    batch.maxInstance = maxInstance;
    batch.count = 0;

    // --- GPU リソース確保 ---
    batch.gpuBuffer = CreateBufferResource(
        DirectXEngine::GetDevice(),
        sizeof(InstanceData) * maxInstance);

    // --- CPU 側から書き込むために永続 Map ---
    batch.gpuBuffer->Map(0, nullptr, reinterpret_cast<void**>(&batch.cpuPtr));
    for (uint32_t i = 0; i < maxInstance; ++i) {
        batch.cpuPtr[i].WVP = Matrix4x4::Identity();
        batch.cpuPtr[i].World = Matrix4x4::Identity();
        batch.cpuPtr[i].WorldInvT = Matrix4x4::Identity();
        batch.cpuPtr[i].color = Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
    }

    // --- SRV を作成して Heap へ登録 ---
    batch.srvIndex = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
    SrvManager::GetInstance()->CreateSRVforStructuredBuffer(
        batch.srvIndex, batch.gpuBuffer.Get(), maxInstance, sizeof(InstanceData));

    objBatches_[model] = std::move(batch);
}

void ModelInstanceRenderer::AnimaReserveBatch(Model* model, uint32_t maxInstance)
{
    // 二重確保ガード
    if (animaBatches_.contains(model)) return;

    AnimationBatch batch{};
    batch.model = model;
    batch.maxInstance = maxInstance;
    batch.count = 0;

    // --- GPU リソース確保 ---
    batch.gpuBuffer = CreateBufferResource(
        DirectXEngine::GetDevice(),
        sizeof(InstanceData) * maxInstance);

    // --- CPU 側から書き込むために永続 Map ---
    batch.gpuBuffer->Map(0, nullptr, reinterpret_cast<void**>(&batch.cpuPtr));
    for (uint32_t i = 0; i < maxInstance; ++i) {
        batch.cpuPtr[i].WVP = Matrix4x4::Identity();
        batch.cpuPtr[i].World = Matrix4x4::Identity();
        batch.cpuPtr[i].WorldInvT = Matrix4x4::Identity();
        batch.cpuPtr[i].color = Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
    }

    // --- SRV を作成して Heap へ登録 ---
    batch.srvIndex = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
    SrvManager::GetInstance()->CreateSRVforStructuredBuffer(
        batch.srvIndex, batch.gpuBuffer.Get(), maxInstance, sizeof(InstanceData));

    animaBatches_[model] = std::move(batch);
}

void ModelInstanceRenderer::Push(Object3d* obj)
{
    // 未確保なら Reserve
    Model* model = obj->GetModel();
    if (!objBatches_.contains(model)) { ObjReserveBatch(model); }

    ObjectBatch& batch = objBatches_[model];
    if (batch.count >= batch.maxInstance) return;    // あふれ防止

    // 書き込み先
    batch.objects.push_back(obj);
    InstanceData& dst = batch.cpuPtr[batch.count++];

    // 行列計算 (CPU 側で済ませても VS で WVP かけ直してもどちらでも可)
    dst.World = obj->GetTransform().instanceMatrix_.World;
    dst.WorldInvT = obj->GetTransform().instanceMatrix_.WorldInverseTranspose;
    dst.WVP = obj->GetTransform().instanceMatrix_.WVP;
    dst.color = obj->GetColor();
}

void ModelInstanceRenderer::Push(Animation* anima)
{
    // 未確保なら Reserve
    Model* model = anima->GetModel();
    if (!animaBatches_.contains(model)) { AnimaReserveBatch(model); }

    AnimationBatch& batch = animaBatches_[model];
    if (batch.count >= batch.maxInstance) return;    // あふれ防止

    // 書き込み先
    batch.animations.push_back(anima);
    InstanceData& dst = batch.cpuPtr[batch.count++];

    // 行列計算 (CPU 側で済ませても VS で WVP かけ直してもどちらでも可)
    dst.World = anima->GetTransform().instanceMatrix_.World;
    dst.WorldInvT = anima->GetTransform().instanceMatrix_.WorldInverseTranspose;
    dst.WVP = anima->GetTransform().instanceMatrix_.WVP;
    dst.color = anima->GetColor();
}

void ModelInstanceRenderer::Remove(Object3d* obj)
{
    Model* model = obj->GetModel();
    ObjectBatch& batch = objBatches_[model];

    for (auto it = batch.objects.begin(); it != batch.objects.end();) {
        if (obj == (*it)) {
            it = batch.objects.erase(it);
            --batch.count;
            break;
        }
        ++it;
    }
}

void ModelInstanceRenderer::Remove(Animation* anima)
{
    Model* model = anima->GetModel();
    AnimationBatch& batch = animaBatches_[model];

    for (auto it = batch.animations.begin(); it != batch.animations.end();) {
        if (anima == (*it)) {
            it = batch.animations.erase(it);
            --batch.count;
            break;
        }
        ++it;
    }
}

void ModelInstanceRenderer::ObjUpdate()
{
    for (auto it = objBatches_.begin(); it != objBatches_.end();) {

        ObjectBatch& batch = objBatches_[it->first];
        for (uint32_t i = 0; i < batch.count; ++i) {
            InstanceData& dst = batch.cpuPtr[i];

            // 行列計算 (CPU 側で済ませても VS で WVP かけ直してもどちらでも可)
            dst.World = batch.objects[i]->GetTransform().instanceMatrix_.World;
            dst.WorldInvT = batch.objects[i]->GetTransform().instanceMatrix_.WorldInverseTranspose;
            dst.WVP = batch.objects[i]->GetTransform().instanceMatrix_.WVP;
            dst.color = batch.objects[i]->GetColor();
        }
        ++it;
    }
}

void ModelInstanceRenderer::AnimaUpdate()
{
    for (auto it = animaBatches_.begin(); it != animaBatches_.end();) {

        AnimationBatch& batch = animaBatches_[it->first];
        for (uint32_t i = 0; i < batch.count; ++i) {
            InstanceData& dst = batch.cpuPtr[i];

            // 行列計算 (CPU 側で済ませても VS で WVP かけ直してもどちらでも可)
            dst.World = batch.animations[i]->GetTransform().instanceMatrix_.World;
            dst.WorldInvT = batch.animations[i]->GetTransform().instanceMatrix_.WorldInverseTranspose;
            dst.WVP = batch.animations[i]->GetTransform().instanceMatrix_.WVP;
            dst.color = batch.animations[i]->GetColor();
        }
        ++it;
    }
}

void ModelInstanceRenderer::AllDraw()
{
    auto* commandList = DirectXEngine::GetCommandList();

    for (auto& [model, batch] : objBatches_) {
        if (batch.count == 0) continue;

        /* ---------- IA 共通バインド ---------- */
        for (auto& object : batch.objects) {
            ObjUpdate();
            object->Draw();
        }
        model->BindBuffers(false);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, batch.srvIndex);

        /* ---------- Mesh ループ ---------- */
        const auto& mesh = model->GetModelData().meshes;
        for (uint32_t i = 0; i < mesh.size(); ++i) {
            model->BindMaterial(i);
            commandList->DrawIndexedInstanced(
                mesh[i].indexCount, batch.count,
                mesh[i].indexStart, 0, 0);
        }
    }

    //for (auto& [model, batch] : animaBatches_) {
    //    if (batch.count == 0) continue;

    //    /* ---------- IA 共通バインド ---------- */
    //    for (auto& anima : batch.animations) {
    //        AnimaUpdate();
    //        anima->Draw();
    //    }
    //    model->BindBuffers(true);
    //    SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, batch.srvIndex);

    //    /* ---------- Mesh ループ ---------- */
    //    const auto& mesh = model->GetModelData().meshes;
    //    for (uint32_t i = 0; i < mesh.size(); ++i) {
    //        model->BindMaterial(i);
    //        commandList->DrawIndexedInstanced(
    //            mesh[i].indexCount, batch.count,
    //            mesh[i].indexStart, 0, 0);
    //    }
    //}
}
