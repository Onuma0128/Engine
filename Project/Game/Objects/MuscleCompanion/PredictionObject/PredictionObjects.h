#pragma once
#include <memory>
#include <array>

#include "Object3d.h"

// 前方宣言
class Player;
class CompanionAdjustItem;

/// <summary>
/// 仲間の予測オブジェクトクラス
/// </summary>
class PredictionObjects
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="offset"></param>
	void Update(const Vector3& start, const Vector3& goal);

	/// <summary>
	/// 描画するか決める
	/// </summary>
	/// <param name="isDraw"></param>
	void SetDraw(bool isDraw);

	/// <summary>
	/// セッター
	/// </summary>
	void SetItems(CompanionAdjustItem* items) { items_ = items; }

private:

	// 調整項目のポインタ
	CompanionAdjustItem* items_ = nullptr;

	// オブジェクトを何個作成するか
	static const size_t objSize_ = 5;
	std::array<std::unique_ptr<Object3d>, objSize_> predictionObjects_;
	
};
