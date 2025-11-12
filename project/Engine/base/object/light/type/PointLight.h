#pragma once

#include "BaseLight.h"

#include "Vector3.h"
#include "Vector4.h"

/// <summary>
/// ポイントライトクラス
/// </summary>
class PointLight : public BaseLight
{
public:

	// ポイントライト用のデータ構造体
	struct PointLightData {
		Vector4 color;
		Vector3 position;
		float intensity;
		float radius;
		float decay;
		float padding[2];
	};

	// 初期化
	void Initialize(DirectXEngine* dxEngine) override;

	// 更新
	void Update() override;

	// デバッグ用のImGui表示
	void Debug_ImGui() override;

	// ライト用のデータ作成
	void MakeLightData() override;

	// ライト用の行列作成
	void BuildMatricesCoverAll(
		const Vector3& sceneMin,
		const Vector3& sceneMax,
		uint32_t shadowW, uint32_t shadowH) override {};

private:

	PointLightData* lightData_ = nullptr;

};

