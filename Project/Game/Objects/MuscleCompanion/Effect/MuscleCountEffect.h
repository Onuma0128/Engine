#pragma once

#include <memory>

#include "PrimitiveDrawr.h"
#include "Vector3.h"

/// <summary>
/// 発射できる仲間の数を表示するエフェクトクラス
/// </summary>
class MuscleCountEffect
{
	public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="position"></param>
	void Update(const Vector3& position);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetEffect(uint32_t maxCount,uint32_t nowCount) {
		timer_ = 1.0f;
		maxCount_ = maxCount;
		nowCount_ = nowCount;
	}

private:

	// 仲間の数を表示するエフェクト
	std::unique_ptr<PrimitiveDrawr> maxCountEffect_ = nullptr;
	std::unique_ptr<PrimitiveDrawr> nowCountEffect_ = nullptr;
	std::unique_ptr<PrimitiveDrawr> catEffect_ = nullptr;

	uint32_t maxCount_ = 0;
	uint32_t nowCount_ = 0;

	float timer_ = 0.0f;
};

