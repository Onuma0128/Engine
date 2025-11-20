#include "WorldTransform.h"

#include "DirectXEngine.h"

#include "Camera.h"
#include "CameraManager.h"
#include "Model.h"

#include "CreateBufferResource.h"

WorldTransform::WorldTransform()
{
    
}

void WorldTransform::TransferMatrix(const Model* model)
{
    matWorld_ = Matrix4x4::Affine(scale_, rotation_, translation_);
    if (parent_) {
        matWorld_ = matWorld_ * parent_->matWorld_;
    }
    Matrix4x4 worldViewMatrixObject = matWorld_ * CameraManager::GetInstance()->GetActiveCamera()->GetViewMatrix(); // カメラから見たワールド座標に変換
    Matrix4x4 worldViewProjectionMatrixObject = worldViewMatrixObject * CameraManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix(); // 射影行列を適用してワールドビュープロジェクション行列を計算
    instanceMatrix_.WVP = model->GetModelData().rootNode.localMatrix * worldViewProjectionMatrixObject; // ワールドビュープロジェクション行列を更新
    instanceMatrix_.World = model->GetModelData().rootNode.localMatrix * matWorld_; // ワールド座標行列を更新
    instanceMatrix_.WorldInverseTranspose = model->GetModelData().rootNode.localMatrix * Matrix4x4::Inverse(matWorld_).Transpose();
}

void WorldTransform::TransferMatrix(const Matrix4x4& localMatrix)
{
    matWorld_ = Matrix4x4::Affine(scale_, rotation_, translation_);
    if (parent_) {
        matWorld_ = matWorld_ * parent_->matWorld_;
    }
    Matrix4x4 worldViewMatrixObject = matWorld_ * CameraManager::GetInstance()->GetActiveCamera()->GetViewMatrix(); // カメラから見たワールド座標に変換
    Matrix4x4 worldViewProjectionMatrixObject = worldViewMatrixObject * CameraManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix(); // 射影行列を適用してワールドビュープロジェクション行列を計算
    instanceMatrix_.WVP = localMatrix * worldViewProjectionMatrixObject; // ワールドビュープロジェクション行列を更新
    instanceMatrix_.World = localMatrix * matWorld_; // ワールド座標行列を更新
    instanceMatrix_.WorldInverseTranspose = Matrix4x4::Inverse(matWorld_).Transpose();
}
