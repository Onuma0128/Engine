#pragma once

#include <array>
#define NOMINMAX

#include "BaseLight.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

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

	// ライト用の行列作成
	void BuildMatricesCoverAll(
		const Vector3& sceneMin,
		const Vector3& sceneMax,
		uint32_t shadowW, uint32_t shadowH) override;

private:

	void BuildViewByTargetEye(const Vector3& Ptarget, const Vector3& Peye, float lengthScale = 1.0f);

	void BuildViewByTargetLength(const Vector3& Ptarget, float L);

private:

	DirectionalLightData* lightData_ = nullptr;

	float radius_ = 1.0f;
	float radiusScale_ = 4.0f;
	Vector3 center_ = Vector3::ExprZero;

};