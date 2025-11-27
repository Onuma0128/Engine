#include "ModelInstanceRenderer.h"

#include "DirectXEngine.h"
#include "SrvManager.h"
#include "TextureManager.h"
#include "PipelineState.h"
#include "PipelineStruct.h"
#include "PostEffectType.h"
#include "LightManager.h"

#include "Object3d.h"
#include "Animation.h"
#include "Model.h"
#include "WorldTransform.h"

#include "CreateBufferResource.h"


void ModelInstanceRenderer::Initialize()
{
    objMaskPipelineState_ = DirectXEngine::GetPipelineState()->GetPipelineState(
        PipelineType::kObjectOutLineMask,
        PostEffectType::kNone,
        BlendMode::kBlendModeNone);
    objMaskRootSignature_ = DirectXEngine::GetPipelineState()->GetRootSignature(
        PipelineType::kObjectOutLineMask,
        PostEffectType::kNone,
        BlendMode::kBlendModeNone);

    animaMaskPipelineState_ = DirectXEngine::GetPipelineState()->GetPipelineState(
        PipelineType::kAnimationOutLineMask ,
        PostEffectType::kNone,
        BlendMode::kBlendModeNone);
    animaMaskRootSignature_ = DirectXEngine::GetPipelineState()->GetRootSignature(
        PipelineType::kAnimationOutLineMask,
        PostEffectType::kNone,
        BlendMode::kBlendModeNone);

    objShadowMapPipelineState_ = DirectXEngine::GetPipelineState()->GetPipelineState(
        PipelineType::kObjectShadowMapDepth,
        PostEffectType::kNone,
        BlendMode::kBlendModeNormal);
    objShadowMapRootSignature_ = DirectXEngine::GetPipelineState()->GetRootSignature(
        PipelineType::kObjectShadowMapDepth,
        PostEffectType::kNone,
        BlendMode::kBlendModeNormal);

    animationShadowMapPipelineState_ = DirectXEngine::GetPipelineState()->GetPipelineState(
        PipelineType::kAnimationShadowMapDepth,
        PostEffectType::kNone,
        BlendMode::kBlendModeNormal);
    animationShadowMapRootSignature_ = DirectXEngine::GetPipelineState()->GetRootSignature(
        PipelineType::kAnimationShadowMapDepth,
        PostEffectType::kNone,
        BlendMode::kBlendModeNormal);

    lightVpBuffer_ = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(Matrix4x4));
	lightVpBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));
    lightData_->lightVP = Matrix4x4::Identity();
}

/// =====================================================================
///                           Object3d_Resource
/// =====================================================================

void ModelInstanceRenderer::ObjReserveBatch(Object3d* object, uint32_t maxInstance)
{
    // 二重確保ガード
    if (objBatches_.contains(object->GetModel())) return;

    ObjectBatch batch{};
    batch.model = object->GetModel();
    batch.maxInstance = maxInstance;
    batch.count = 0;

    // --- GPU リソース確保 WorldMatrix ---
    batch.worldMatrixBuffer = CreateBufferResource(
        DirectXEngine::GetDevice(),
        sizeof(InstanceData) * maxInstance);
    // --- GPU リソース確保 Material ---
    batch.materialBuffer = CreateBufferResource(
        DirectXEngine::GetDevice(),
        sizeof(Material) * maxInstance);


    // --- CPU 側から書き込むために永続 WorldMatrix ---
    batch.worldMatrixBuffer->Map(0, nullptr, reinterpret_cast<void**>(&batch.instanceData));
    for (uint32_t i = 0; i < maxInstance; ++i) {
        batch.instanceData[i].WVP = Matrix4x4::Identity();
        batch.instanceData[i].World = Matrix4x4::Identity();
        batch.instanceData[i].WorldInvT = Matrix4x4::Identity();
    }
    // --- CPU 側から書き込むために永続 Material ---
    batch.materialBuffer->Map(0, nullptr, reinterpret_cast<void**>(&batch.materialData));
    for (uint32_t i = 0; i < maxInstance; ++i) {
        batch.materialData[i].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        batch.materialData[i].enableDraw = true;
        batch.materialData[i].enableLighting = true;
        batch.materialData[i].outlineMask = false;
        batch.materialData[i].uvTransform = Matrix4x4::Identity();
        batch.materialData[i].shininess = 20.0f;
        batch.materialData[i].environmentCoefficient = 0;
        batch.materialData[i].shadowMap = true;
    }

    // --- SRV を作成して Heap へ登録 WorldMatrix ---
    batch.instSrvIndex = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
    SrvManager::GetInstance()->CreateSRVforStructuredBuffer(
        batch.instSrvIndex, batch.worldMatrixBuffer.Get(), maxInstance, sizeof(InstanceData));
    // --- SRV を作成して Heap へ登録 Material ---
    batch.materialSrvIndex = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
    SrvManager::GetInstance()->CreateSRVforStructuredBuffer(
        batch.materialSrvIndex, batch.materialBuffer.Get(), maxInstance, sizeof(Material));

    objBatches_[object->GetModel()] = std::move(batch);
}

/// =====================================================================
///                           Animation_Resource
/// =====================================================================

void ModelInstanceRenderer::AnimationReserveBatch(Animation* animation, uint32_t maxInstance)
{
    // 二重確保ガード
    if (animationBatches_.contains(animation->GetModel())) return;

    AnimationBatch batch{};
    batch.model = animation->GetModel();
    batch.maxInstance = maxInstance;
    batch.count = 0;

    // --- GPU リソース確保 WorldMatrix ---
    batch.worldMatrixBuffer = CreateBufferResource(
        DirectXEngine::GetDevice(),
        sizeof(InstanceData) * maxInstance);
    // --- GPU リソース確保 Material ---
    batch.materialBuffer = CreateBufferResource(
        DirectXEngine::GetDevice(),
        sizeof(Material) * maxInstance);
    // --- GPU リソース確保 Joint ---
    batch.jointBuffer = CreateBufferResource(
        DirectXEngine::GetDevice(),
        sizeof(JointCount));
    // --- GPU リソース確保 MatrixPalette ---
    batch.paletteBuffer = CreateBufferResource(
        DirectXEngine::GetDevice(),
        sizeof(WellForGPU) * maxInstance * animation->GetJointSize());


    // --- CPU 側から書き込むために永続 WorldMatrix ---
    batch.worldMatrixBuffer->Map(0, nullptr, reinterpret_cast<void**>(&batch.instanceData));
    for (uint32_t i = 0; i < maxInstance; ++i) {
        batch.instanceData[i].WVP = Matrix4x4::Identity();
        batch.instanceData[i].World = Matrix4x4::Identity();
        batch.instanceData[i].WorldInvT = Matrix4x4::Identity();
    }
    // --- CPU 側から書き込むために永続 Material ---
    batch.materialBuffer->Map(0, nullptr, reinterpret_cast<void**>(&batch.materialData));
    for (uint32_t i = 0; i < maxInstance; ++i) {
        batch.materialData[i].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        batch.materialData[i].enableDraw = true;
        batch.materialData[i].enableLighting = true;
        batch.materialData[i].outlineMask = false;
        batch.materialData[i].uvTransform = Matrix4x4::Identity();
        batch.materialData[i].shininess = 20.0f;
        batch.materialData[i].environmentCoefficient = 0;
        batch.materialData[i].shadowMap = true;
    }
    // --- CPU 側から書き込むために永続 Joint ---
    batch.jointBuffer->Map(0, nullptr, reinterpret_cast<void**>(&batch.jointData));
    batch.jointData->jointCount = static_cast<uint32_t>(animation->GetJointSize());
    // --- CPU 側から書き込むために永続 Palette ---
    batch.paletteBuffer->Map(0, nullptr, reinterpret_cast<void**>(&batch.paletteData));
    for (uint32_t i = 0; i < maxInstance; ++i) {
        batch.paletteData[i].skeletonSpaceMatrix = Matrix4x4::Identity();
        batch.paletteData[i].skeletonSpaceInverseTransposeMatrix = Matrix4x4::Identity();
    }

    // --- SRV を作成して Heap へ登録 WorldMatrix ---
    batch.instSrvIndex = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
    SrvManager::GetInstance()->CreateSRVforStructuredBuffer(
        batch.instSrvIndex, batch.worldMatrixBuffer.Get(), maxInstance, sizeof(InstanceData));
    // --- SRV を作成して Heap へ登録 Material ---
    batch.materialSrvIndex = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
    SrvManager::GetInstance()->CreateSRVforStructuredBuffer(
        batch.materialSrvIndex, batch.materialBuffer.Get(), maxInstance, sizeof(Material));
    // --- SRV を作成して Heap へ登録 Palette ---
    batch.paletteSrvIndex = SrvManager::GetInstance()->Allocate() + TextureManager::kSRVIndexTop;
    SrvManager::GetInstance()->CreateSRVforStructuredBuffer(
        batch.paletteSrvIndex, batch.paletteBuffer.Get(), maxInstance * static_cast<UINT>(animation->GetJointSize()), sizeof(WellForGPU));


    animationBatches_[animation->GetModel()] = std::move(batch);
}

/// =====================================================================
///                      Object3d_Push & Remove
/// =====================================================================

void ModelInstanceRenderer::Push(Object3d* obj)
{
    // 未確保なら Reserve
    Model* model = obj->GetModel();
    if (!objBatches_.contains(model)) { ObjReserveBatch(obj); }

    ObjectBatch& batch = objBatches_[model];
    if (batch.count >= batch.maxInstance) return;    // あふれ防止

    batch.objects.push_back(obj);
    // 書き込み先 WorldMatrix
    InstanceData& matrix = batch.instanceData[batch.count];
    // 行列計算 (CPU 側で済ませても VS で WVP かけ直してもどちらでも可)
    matrix.World = obj->GetTransform().instanceMatrix_.World;
    matrix.WorldInvT = obj->GetTransform().instanceMatrix_.WorldInverseTranspose;
    matrix.WVP = obj->GetTransform().instanceMatrix_.WVP;

    // 書き込み先 Material
    Material& material = batch.materialData[batch.count];
    // Materialを代入
    material = obj->GetMaterial();

    ++batch.count;
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

/// =====================================================================
///                     Animation_Push & Remove
/// =====================================================================

void ModelInstanceRenderer::Push(Animation* animation)
{
    // 未確保なら Reserve
    Model* model = animation->GetModel();
    if (!animationBatches_.contains(model)) { AnimationReserveBatch(animation); }

    AnimationBatch& batch = animationBatches_[model];
    if (batch.count >= batch.maxInstance) return;    // あふれ防止

    batch.animations.push_back(animation);
    // 書き込み先 WorldMatrix
    InstanceData& matrix = batch.instanceData[batch.count];
    // 行列計算 (CPU 側で済ませても VS で WVP かけ直してもどちらでも可)
    matrix.World = animation->GetTransform().instanceMatrix_.World;
    matrix.WorldInvT = animation->GetTransform().instanceMatrix_.WorldInverseTranspose;
    matrix.WVP = animation->GetTransform().instanceMatrix_.WVP;
    // 書き込み先 Material
    Material& material = batch.materialData[batch.count];
    material = animation->GetMaterial();

    ++batch.count;
}

void ModelInstanceRenderer::Remove(Animation* animation)
{
    Model* model = animation->GetModel();
    AnimationBatch& batch = animationBatches_[model];

    for (auto it = batch.animations.begin(); it != batch.animations.end();) {
        if (animation == (*it)) {
            it = batch.animations.erase(it);
            --batch.count;
            break;
        }
        ++it;
    }
}

/// =====================================================================
///                          Object3d_Update
/// =====================================================================

void ModelInstanceRenderer::ObjUpdate()
{
    for (auto it = objBatches_.begin(); it != objBatches_.end();) {

        ObjectBatch& batch = objBatches_[it->first];
        for (uint32_t i = 0; i < batch.count; ++i) {

            // 行列計算
            InstanceData& matrix = batch.instanceData[i];
            matrix.World = batch.objects[i]->GetTransform().instanceMatrix_.World;
            matrix.WorldInvT = batch.objects[i]->GetTransform().instanceMatrix_.WorldInverseTranspose;
            matrix.WVP = batch.objects[i]->GetTransform().instanceMatrix_.WVP;
            // Materialを代入
            Material& material = batch.materialData[i];
            material = batch.objects[i]->GetMaterial();
        }
        ++it;
    }
}

/// =====================================================================
///                         Animation_Update
/// =====================================================================

void ModelInstanceRenderer::AnimationUpdate()
{
    for (auto it = animationBatches_.begin(); it != animationBatches_.end();) {

        AnimationBatch& batch = animationBatches_[it->first];
        for (uint32_t i = 0; i < batch.count; ++i) {

            // 行列計算
            InstanceData& matrix = batch.instanceData[i];
            matrix.World = batch.animations[i]->GetTransform().instanceMatrix_.World;
            matrix.WorldInvT = batch.animations[i]->GetTransform().instanceMatrix_.WorldInverseTranspose;
            matrix.WVP = batch.animations[i]->GetTransform().instanceMatrix_.WVP;
            // Materialを代入
            Material& material = batch.materialData[i];
            material = batch.animations[i]->GetMaterial();
            // 描画するか
            if (material.enableDraw) {
                // 描画をしているならPaletteの更新
                size_t jointSize = batch.animations[i]->GetJointSize();
                for (size_t jointIndex = 0; jointIndex < jointSize; ++jointIndex) {
                    const size_t dst = i * jointSize + jointIndex;
                    WellForGPU& wellForGPU = batch.paletteData[dst];
                    wellForGPU.skeletonSpaceMatrix = batch.animations[i]->GetWellForGPU()[jointIndex].skeletonSpaceMatrix;
                    wellForGPU.skeletonSpaceInverseTransposeMatrix =
                        batch.animations[i]->GetWellForGPU()[jointIndex].skeletonSpaceInverseTransposeMatrix;
                }
            }
        }
        ++it;
    }
}

void ModelInstanceRenderer::AllDrawShadowDepth()
{
    auto* commandList = DirectXEngine::GetCommandList();

    ObjUpdate();

    if (!objBatches_.empty()) {
        commandList->SetPipelineState(objShadowMapPipelineState_.Get());
        commandList->SetGraphicsRootSignature(objShadowMapRootSignature_.Get());
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

	lightData_->lightVP = LightManager::GetInstance()->GetDirectionalLight()->GetLightVP();

    for (auto& [model, batch] : objBatches_) {
        if (batch.count == 0) continue;

        model->BindBuffers(false);
		commandList->SetGraphicsRootConstantBufferView(0, lightVpBuffer_->GetGPUVirtualAddress());
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, batch.instSrvIndex);
		SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, batch.materialSrvIndex);

        /* ---------- Mesh ループ ---------- */
        const auto& mesh = model->GetMeshData();
        for (uint32_t i = 0; i < mesh.size(); ++i) {
            commandList->DrawIndexedInstanced(
                mesh[i].indexCount, batch.count,
                mesh[i].indexStart, 0, 0);
        }
    }

    AnimationUpdate();

    if (!animationBatches_.empty()) {
        commandList->SetPipelineState(animationShadowMapPipelineState_.Get());
        commandList->SetGraphicsRootSignature(animationShadowMapRootSignature_.Get());
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    for (auto& [model, batch] : animationBatches_) {
        if (batch.count == 0) continue;

        batch.animations.back()->SetVertexBuffer();
        model->BindBuffers(true);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(0, batch.instSrvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, batch.paletteSrvIndex);
        commandList->SetGraphicsRootConstantBufferView(2, batch.jointBuffer->GetGPUVirtualAddress());
        commandList->SetGraphicsRootConstantBufferView(3, lightVpBuffer_->GetGPUVirtualAddress());
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(4, batch.materialSrvIndex);

        /* ---------- Mesh ループ ---------- */
        const auto& mesh = model->GetMeshData();
        for (uint32_t i = 0; i < mesh.size(); ++i) {
            commandList->DrawIndexedInstanced(
                mesh[i].indexCount, batch.count,
                mesh[i].indexStart, 0, 0);
        }
    }
}

void ModelInstanceRenderer::AllDrawOutlineMask() {
    auto* commandList = DirectXEngine::GetCommandList();

    if (!objBatches_.empty()) {
        commandList->SetPipelineState(objMaskPipelineState_.Get());
        commandList->SetGraphicsRootSignature(objMaskRootSignature_.Get());
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    // === Object3d バッチ ===
    for (auto& [model, batch] : objBatches_) {
        if (batch.count == 0) continue;

        model->BindBuffers(false);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(0, batch.instSrvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, batch.materialSrvIndex);
        commandList->SetGraphicsRootConstantBufferView(2, lightVpBuffer_->GetGPUVirtualAddress());

        const auto& mesh = model->GetMeshData();
        for (uint32_t i = 0; i < mesh.size(); ++i) {
            commandList->DrawIndexedInstanced(mesh[i].indexCount, batch.count, mesh[i].indexStart, 0, 0);
        }
    }

    if (!animationBatches_.empty()) {
        commandList->SetPipelineState(animaMaskPipelineState_.Get());
        commandList->SetGraphicsRootSignature(animaMaskRootSignature_.Get());
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    // === Animation バッチ ===
    for (auto& [model, batch] : animationBatches_) {
        if (batch.count == 0) continue;

        model->BindBuffers(true);
        batch.animations[0]->SetVertexBuffer();
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(0, batch.instSrvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, batch.paletteSrvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, batch.materialSrvIndex);
        commandList->SetGraphicsRootConstantBufferView(3, batch.jointBuffer->GetGPUVirtualAddress());
        commandList->SetGraphicsRootConstantBufferView(4, lightVpBuffer_->GetGPUVirtualAddress());

        const auto& mesh = model->GetMeshData();
        for (uint32_t i = 0; i < mesh.size(); ++i) {
            commandList->DrawIndexedInstanced(mesh[i].indexCount, batch.count, mesh[i].indexStart, 0, 0);
        }
    }
}


void ModelInstanceRenderer::AllDraw()
{
    auto* commandList = DirectXEngine::GetCommandList();

    for (auto& [model, batch] : objBatches_) {
        if (batch.count == 0) continue;

        /* ---------- IA 共通バインド ---------- */
        for (auto& object : batch.objects) {
            object->Draw();
        }
        model->BindBuffers(false);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(0, batch.materialSrvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, batch.instSrvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(9, DirectXEngine::GetShadowMap()->GetShadowMapSrvIndex());
        commandList->SetGraphicsRootConstantBufferView(10, lightVpBuffer_->GetGPUVirtualAddress());

        /* ---------- Mesh ループ ---------- */
        const auto& mesh = model->GetMeshData();
        for (uint32_t i = 0; i < mesh.size(); ++i) {
            model->BindMaterial(mesh[i].materialIndex);
            commandList->DrawIndexedInstanced(
                mesh[i].indexCount, batch.count,
                mesh[i].indexStart, 0, 0);
        }
    }

    for (auto& [model, batch] : animationBatches_) {
        if (batch.count == 0) continue;

        /* ---------- IA 共通バインド ---------- */
        for (auto& animation : batch.animations) {
            animation->Draw();
        }
        model->BindBuffers(true);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(0, batch.materialSrvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, batch.instSrvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(3, batch.paletteSrvIndex);
        commandList->SetGraphicsRootConstantBufferView(10, batch.jointBuffer->GetGPUVirtualAddress());
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(11, DirectXEngine::GetShadowMap()->GetShadowMapSrvIndex());
        commandList->SetGraphicsRootConstantBufferView(12, lightVpBuffer_->GetGPUVirtualAddress());

        /* ---------- Mesh ループ ---------- */
        const auto& mesh = model->GetMeshData();
        for (uint32_t i = 0; i < mesh.size(); ++i) {
            model->BindMaterial(mesh[i].materialIndex);
            commandList->DrawIndexedInstanced(
                mesh[i].indexCount, batch.count,
                mesh[i].indexStart, 0, 0);
        }
    }

}
