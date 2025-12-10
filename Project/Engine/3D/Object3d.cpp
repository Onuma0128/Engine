#include "Object3d.h"

#include "DirectXEngine.h"
#include "ModelInstanceRenderer.h"

#include "CameraManager.h"
#include "Camera.h"
#include "LightManager.h"
#include "ModelManager.h"
#include "Model.h"

#include "CreateBufferResource.h"

void Object3d::Initialize(const std::string& filePath)
{
    object3dBase_ = std::make_unique<Object3dBase>();
    object3dBase_->Initialize();

    transform_ = WorldTransform();

    SetModel(filePath);

    MakeMaterialData();
}

void Object3d::SetSceneRenderer()
{
    ModelInstanceRenderer::GetInstance()->Push(this);
}

void Object3d::RemoveRenderer()
{
    ModelInstanceRenderer::GetInstance()->Remove(this);
}

void Object3d::Update()
{
    transform_.TransferMatrix(model_->GetLocalMatrix());
}

void Object3d::Draw()
{
    object3dBase_->DrawBase();

    auto commandList = DirectXEngine::GetCommandList();
    commandList->SetGraphicsRootConstantBufferView(3, LightManager::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(4, LightManager::GetInstance()->GetPointLightResource()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(5, LightManager::GetInstance()->GetSpotLightResource()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(6, CameraManager::GetInstance()->GetCameraResource()->GetGPUVirtualAddress());
}

void Object3d::SetModel(const std::string& filePath)
{
    model_ = ModelManager::GetInstance()->FindModel(filePath);
}

void Object3d::SetTexture(const std::string& directoryPath, const std::string& filePath)
{
    model_->SetTexture(directoryPath, filePath);
}

void Object3d::SetTexture_ENV(const std::string& directoryPath, const std::string& filePath)
{
    model_->SetTexture_ENV(directoryPath, filePath);
}

void Object3d::SetColor(const Vector4& color)
{
    materialData_.color = color;
}

void Object3d::MakeMaterialData()
{
    materialData_.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData_.enableDraw = true;
    materialData_.enableLighting = true;
    materialData_.outlineMask = false;
    materialData_.outlineSceneColor = false;
    materialData_.uvTransform = Matrix4x4::Identity();
    materialData_.shininess = 20.0f;
    materialData_.environmentCoefficient = 0;
    materialData_.shadowMap = true;
}