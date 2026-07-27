#include "Object3d.h"

#include "DirectXEngine.h"
#include "ModelInstanceRenderer.h"

#include "CameraManager.h"
#include "Camera.h"
#include "LightManager.h"
#include "ModelManager.h"
#include "Model.h"

#include "CreateBufferResource.h"

void NumaEngine::Object3d::Initialize(const std::string& filePath)
{
    object3dBase_ = std::make_unique<Object3dBase>();
    object3dBase_->Initialize();

    transform_ = WorldTransform();

    SetModel(filePath);

    MakeMaterialData();
}

void NumaEngine::Object3d::SetSceneRenderer()
{
    NumaEngine::ModelInstanceRenderer::GetInstance()->Push(this);
}

void NumaEngine::Object3d::RemoveRenderer()
{
    NumaEngine::ModelInstanceRenderer::GetInstance()->Remove(this);
}

void NumaEngine::Object3d::Update()
{
    transform_.TransferMatrix(model_->GetLocalMatrix());
}

void NumaEngine::Object3d::Draw()
{
    object3dBase_->DrawBase();

    auto commandList = NumaEngine::DirectXEngine::GetCommandList();
    commandList->SetGraphicsRootConstantBufferView(3, LightManager::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(4, LightManager::GetInstance()->GetPointLightResource()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(5, LightManager::GetInstance()->GetSpotLightResource()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(6, CameraManager::GetInstance()->GetCameraResource()->GetGPUVirtualAddress());
}

void NumaEngine::Object3d::SetModel(const std::string& filePath)
{
    model_ = NumaEngine::ModelManager::GetInstance()->FindModel(filePath);
}

void NumaEngine::Object3d::SetTexture(const std::string& directoryPath, const std::string& filePath)
{
    model_->SetTexture(directoryPath, filePath);
}

void NumaEngine::Object3d::SetTexture_ENV(const std::string& directoryPath, const std::string& filePath)
{
    model_->SetTexture_ENV(directoryPath, filePath);
}

void NumaEngine::Object3d::SetColor(const NumaEngine::Vector4& color)
{
    materialData_.color = color;
}

void NumaEngine::Object3d::MakeMaterialData()
{
    materialData_.color = NumaEngine::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData_.enableDraw = true;
    materialData_.enableLighting = true;
    materialData_.outlineMask = false;
    materialData_.outlineSceneColor = false;
    materialData_.uvTransform = NumaEngine::Matrix4x4::Identity();
    materialData_.shininess = 20.0f;
    materialData_.environmentCoefficient = 0;
    materialData_.shadowMap = true;
}

