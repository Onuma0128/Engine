#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")

#include <memory>

#include "type/BaseLight.h"

class DirectXEngine;

/// <summary>
/// ライト管理クラス
/// </summary>
class LightManager
{
private:

	// シングルトンインスタンス
	static LightManager* instance_;

	LightManager() = default;
	~LightManager() = default;
	LightManager(LightManager&) = delete;
	LightManager& operator=(LightManager&) = delete;

public:

	/*==================== メンバ関数 ====================*/

	// シングルトンインスタンスの取得
	static LightManager* GetInstance();

	// 初期化
	void Initialize(DirectXEngine* dxEngine);

	// 更新
	void Update();

	// デバッグ用ImGui表示
	void Debug_ImGui();

	// 終了処理
	void Finalize();

	/*==================== アクセッサー ====================*/

	// 平行光源のリソース取得
	BaseLight* GetDirectionalLight() const { return directionalLight_.get(); }
	ID3D12Resource* GetDirectionalLightResource()const { return directionalLight_->GetResource(); }
	// ポイントライトのリソース取得
	ID3D12Resource* GetPointLightResource()const { return pointLight_->GetResource(); }
	// スポットライトのリソース取得
	ID3D12Resource* GetSpotLightResource()const { return spotLight_->GetResource(); }

private:

	DirectXEngine* dxEngine_ = nullptr;

	/*==================== 平行光源 ====================*/

	std::unique_ptr<BaseLight> directionalLight_ = nullptr;

	/*==================== ポイントライト ====================*/

	std::unique_ptr<BaseLight> pointLight_ = nullptr;

	/*==================== スポットライト ====================*/

	std::unique_ptr<BaseLight> spotLight_ = nullptr;

};