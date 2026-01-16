#pragma once

#include <memory>

#include "PrimitiveDrawr.h"

#include "Vector3.h"

// 前方宣言
class CompanionAdjustItem;

/// <summary>
/// 次発射できる仲間の上に出る矢印エフェクトクラス
/// </summary>
class NextArrowEffect
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

	/// <summary>
	/// セッター
	/// </summary>
	void SetItems(CompanionAdjustItem* items) { items_ = items; }
	void SetDraw(bool flag) { arrow_->GetRenderOptions().enabled = flag; }

private:

	// 調整項目のポインタ
	CompanionAdjustItem* items_ = nullptr;

	// 矢印エフェクト
	std::unique_ptr<PrimitiveDrawr> arrow_ = nullptr;
	float animaTime_ = 0.0f;


};

