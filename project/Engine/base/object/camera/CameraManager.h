#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>

#include <vector>

#include "Vector3.h"

using Microsoft::WRL::ComPtr;

class DirectXEngine;
class Camera;

/// <summary>
/// カメラ管理クラス
/// </summary>
class CameraManager
{
private:

	// シングルトンインスタンス
	static CameraManager* instance_;

	CameraManager() = default;
	~CameraManager() = default;
	CameraManager(CameraManager&) = delete;
	CameraManager& operator=(CameraManager&) = delete;

	// GPU用カメラデータ構造体
	struct CameraForGPU {
		Vector3 worldPosition;
	};

public:

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
	void SetCamera(Camera* camera) {
		cameras_.push_back(std::move(camera)); 
	}

	// アクティブカメラ設定
	void SetActiveCamera(uint32_t index) {
		activeCameraIndex_ = index;
	}

	// アクティブカメラ取得
	Camera* GetActiveCamera()const { return cameras_[activeCameraIndex_]; }

	// カメラリソース
	ID3D12Resource* GetCameraResource()const { return cameraResource_.Get(); }

private:

	// カメラデータ作成
	void MakeCameraData();

private:

	DirectXEngine* dxEngine_;

	// カメラを格納 <vector>
	std::vector<Camera*> cameras_;
	// 今どのカメラが動いているかIndex
	uint32_t activeCameraIndex_ = 0;

	/*==================== カメラデータ ====================*/

	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	CameraForGPU* cameraData_ = nullptr;

};

