#include "CameraManager.h"

#include "DirectXEngine.h"
#include "CreateBufferResource.h"

#include "Camera.h"

std::unique_ptr<CameraManager> CameraManager::instance_ = nullptr;

CameraManager* CameraManager::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = std::make_unique<CameraManager>();
	}
	return instance_.get();
}

void CameraManager::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	// カメラリソースの作成
	cameras_.push_back(std::make_shared<Camera>());
	cameras_[activeCameraIndex_]->Initialize();
	MakeCameraData();
}

void CameraManager::Debug_ImGui()
{
	// ImGuiの更新
	cameras_[activeCameraIndex_]->CameraImGui();
}

void CameraManager::Update()
{
	// カメラの更新
	cameras_[activeCameraIndex_]->Update();

	// カメラの座標を転送
	cameraData_->worldPosition = Vector3{}.Transform(cameras_[activeCameraIndex_]->GetWorldMatrix());
}

void CameraManager::Finalize()
{
	instance_ = nullptr;
}

void CameraManager::Clear()
{
	cameras_.clear();
}

void CameraManager::MakeCameraData()
{
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	cameraResource_ = CreateBufferResource(dxEngine_->GetDevice(), sizeof(Vector3));

	// 書き込むためのアドレスを取得
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));

	// 単位行列を書き込んでおく
	cameraData_->worldPosition = Vector3{}.Transform(cameras_[activeCameraIndex_]->GetWorldMatrix());
}
