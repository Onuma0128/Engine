#include "CameraManager.h"

#include "DirectXEngine.h"
#include "CreateBufferResource.h"

#include "Camera.h"

std::unique_ptr<NumaEngine::CameraManager> NumaEngine::CameraManager::instance_ = nullptr;

NumaEngine::CameraManager* NumaEngine::CameraManager::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = std::make_unique<NumaEngine::CameraManager>();
	}
	return instance_.get();
}

void NumaEngine::CameraManager::Initialize(NumaEngine::DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	// カメラリソースの作成
	cameras_.push_back(std::make_shared<NumaEngine::Camera>());
	cameras_[activeCameraIndex_]->Initialize();
	MakeCameraData();
}

void NumaEngine::CameraManager::Debug_ImGui()
{
	// ImGuiの更新
	cameras_[activeCameraIndex_]->CameraImGui();
}

void NumaEngine::CameraManager::Update()
{
	// カメラの更新
	cameras_[activeCameraIndex_]->Update();

	// カメラの座標を転送
	cameraData_->worldPosition = NumaEngine::Vector3{}.Transform(cameras_[activeCameraIndex_]->GetWorldMatrix());
}

void NumaEngine::CameraManager::Finalize()
{
	instance_ = nullptr;
}

void NumaEngine::CameraManager::Clear()
{
	cameras_.clear();
}

void NumaEngine::CameraManager::MakeCameraData()
{
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	cameraResource_ = NumaEngine::CreateBufferResource(dxEngine_->GetDevice(), sizeof(NumaEngine::Vector3));

	// 書き込むためのアドレスを取得
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));

	// 単位行列を書き込んでおく
	cameraData_->worldPosition = NumaEngine::Vector3{}.Transform(cameras_[activeCameraIndex_]->GetWorldMatrix());
}

