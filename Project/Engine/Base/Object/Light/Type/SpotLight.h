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
		NumaEngine::Vector4 color;
		NumaEngine::Vector3 position;
		float intensity;
		NumaEngine::Vector3 direction;
		float distance;
		float decay;
		float cosAngle;
		float cosFalloffStart;
		float padding;
	};

    // 初期化
	void Initialize(NumaEngine::DirectXEngine* dxEngine) override;

	// 更新
	void Update() override;

	// デバッグ用のImGui表示
	void Debug_ImGui() override;
	
	// ライト用のデータ作成
	void MakeLightData() override;

	// ライト用の行列作成
	void BuildMatricesCoverAll(
		const NumaEngine::Vector3& sceneMin,
		const NumaEngine::Vector3& sceneMax,
		uint32_t shadowW, uint32_t shadowH) override {};

private:

	SpotLightData* lightData_ = nullptr;

};

