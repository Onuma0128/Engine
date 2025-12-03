#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>

#include <memory>
#include <vector>

#include "Vector3.h"

#include "Camera.h"

using Microsoft::WRL::ComPtr;

class DirectXEngine;

/// <summary>
/// カメラ管理クラス
/// </summary>
class CameraManager
{
private:

	// シングルトンインスタンス
	static std::unique_ptr<CameraManager> instance_;

	// GPU用カメラデータ構造体
	struct CameraForGPU {
		Vector3 worldPosition;
	};

public:

	CameraManager() = default;
	~CameraManager() = default;
	CameraManager(CameraManager&) = delete;
	CameraManager& operator=(CameraManager&) = delete;

	// シングルトンインスタンスの取得
	static CameraManager* GetInstance();

	// 初期化
	void Initialize(DirectXEngine* dxEngine);

	// デバッグ用ImGui
	void Debug_ImGui();

	// 更新
	void Update();

	// 終了
	void Finalize();

	// クリア
	void Clear();

	// カメラ設定
	void SetCamera(std::shared_ptr<Camera> camera) {
		cameras_.push_back(camera); 
	}

	// アクティブカメラ設定
	void SetActiveCamera(uint32_t index) {
		activeCameraIndex_ = index;
	}

	// アクティブカメラ取得
	Camera* GetActiveCamera()const { return cameras_[activeCameraIndex_].get(); }

	// カメラリソース
	ID3D12Resource* GetCameraResource()const { return cameraResource_.Get(); }

private:

	// カメラデータ作成
	void MakeCameraData();

private:

	DirectXEngine* dxEngine_;

	// カメラを格納 <vector>
	std::vector<std::shared_ptr<Camera>> cameras_;
	// 今どのカメラが動いているかIndex
	uint32_t activeCameraIndex_ = 0;

	/*==================== カメラデータ ====================*/

	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	CameraForGPU* cameraData_ = nullptr;

};

