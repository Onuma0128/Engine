#include "Object3d.h"

#include "DirectXEngine.h"

#include "CameraManager.h"
#include "Camera.h"
#include "Object3dBase.h"
#include "LightManager.h"
#include "ModelManager.h"
#include "Model.h"

#include "CreateBufferResource.h"

void Object3d::Initialize(const std::string& filePath)
{
    this->object3dBase_ = Object3dBase::GetInstance();

    transform_ = WorldTransform();

    SetModel(filePath);

    MakeMaterialData();
}

void Object3d::Update()
{
    transform_.TransferMatrix(model_);
}

void Object3d::Draw()
{
    object3dBase_->DrawBase();

    auto commandList = object3dBase_->GetDxEngine()->GetCommandList();
    commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(1, transform_.GetConstBuffer()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(3, LightManager::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(4, LightManager::GetInstance()->GetPointLightResource()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(5, LightManager::GetInstance()->GetSpotLightResource()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(6, CameraManager::GetInstance()->GetCameraResource()->GetGPUVirtualAddress());

    if (model_) {
        model_->Draw(false);
    }

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
    materialData_->color = color;
}

void Object3d::MakeMaterialData()
{
    // マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
    materialResource_ = CreateBufferResource(object3dBase_->GetDxEngine()->GetDevice(), sizeof(Material)).Get();
    // 書き込むためのアドレスを取得
    materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
    // 今回は白を書き込んでいく
    materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData_->enableLighting = true;
    materialData_->uvTransform = Matrix4x4::Identity();
    materialData_->shininess = 20.0f;
    materialData_->environmentCoefficient = 0;
}