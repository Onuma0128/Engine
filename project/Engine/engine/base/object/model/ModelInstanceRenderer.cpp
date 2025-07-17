#include "ModelInstanceRenderer.h"

#include "DirectXEngine.h"
#include "SrvManager.h"
#include "TextureManager.h"

#include "Object3d.h"
#include "Animation.h"
#include "Model.h"
#include "WorldTransform.h"

#include "CreateBufferResource.h"

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
        batch.materialData[i].uvTransform = Matrix4x4::Identity();
        batch.materialData[i].shininess = 20.0f;
        batch.materialData[i].environmentCoefficient = 0;
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
        batch.materialData[i].uvTransform = Matrix4x4::Identity();
        batch.materialData[i].shininess = 20.0f;
        batch.materialData[i].environmentCoefficient = 0;
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

int32_t ModelInstanceRenderer::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
    Joint joint;
    joint.name = node.name;
    joint.localMatrix = node.localMatrix;
    joint.skeletonSpaceMatrix = Matrix4x4::Identity();
    joint.transform = node.transform;
    joint.index = int32_t(joints.size());
    joint.parent = parent;
    joints.push_back(joint);
    for (const Node& child : node.children) {
        int32_t childIndex = CreateJoint(child, joint.index, joints);
        joints[joint.index].children.push_back(childIndex);
    }

    return joint.index;
}

void ModelInstanceRenderer::AllDraw()
{
    auto* commandList = DirectXEngine::GetCommandList();

    for (auto& [model, batch] : objBatches_) {
        if (batch.count == 0) continue;

        /* ---------- IA 共通バインド ---------- */
        ObjUpdate();
        for (auto& object : batch.objects) {
            object->Draw();
        }
        model->BindBuffers(false);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(0, batch.materialSrvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, batch.instSrvIndex);

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
        AnimationUpdate();
        for (auto& animation : batch.animations) {
            animation->Draw();
        }
        model->BindBuffers(true);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(0, batch.materialSrvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, batch.instSrvIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(3, batch.paletteSrvIndex);
        commandList->SetGraphicsRootConstantBufferView(10, batch.jointBuffer->GetGPUVirtualAddress());

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
