#pragma once

#include "BaseLight.h"

#include "Vector3.h"
#include "Vector4.h"

/// <summary>
/// 平行光源クラス
/// </summary>
class DirectionalLight : public BaseLight
{
public:

	// 平行光源用データ構造体
	struct DirectionalLightData {
		Vector4 color;
		Vector3 direction;
		float intensity;
	};

	// 初期化
	void Initialize(DirectXEngine* dxEngine) override;

	// 更新
	void Update() override;

	// デバッグ用のImGui表示
	void Debug_ImGui() override;

	// ライト用のデータ作成
	void MakeLightData() override;

private:

	DirectionalLightData* lightData_ = nullptr;

};