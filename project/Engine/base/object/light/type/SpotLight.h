#pragma once

#include "BaseLight.h"

#include "Vector3.h"
#include "Vector4.h"

/// <summary>
/// スポットライトクラス
/// </summary>
class SpotLight : public BaseLight
{
public:

	// スポットライト用のデータ構造体
	struct SpotLightData {
		Vector4 color;
		Vector3 position;
		float intensity;
		Vector3 direction;
		float distance;
		float decay;
		float cosAngle;
		float cosFalloffStart;
		float padding;
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

	SpotLightData* lightData_ = nullptr;

};