#pragma once

#include "JsonFunction.h"

// 仲間全体の調整項目
struct CompanionMainData {
	float speed;				// 移動速度
	float hitPushSpeed;			// 仲間同士の押し出しスピード
	float colliderSize;			// 仲間本体のコライダーサイズ
	Vector3 colliderOffset;		// 仲間本体のコライダー座標

	float searchUpdateTime;		// 仲間の探索を更新する時間
	float searchCancelDistance;	// 仲間の探索をやめる距離
	bool debugSpline;			// 仲間の探索経路デバッグ用

	float dashSpeed;			// 攻撃時の移動速度
};

/// <summary>
/// 仲間の調整項目を調整、ロード、保存管理するクラス
/// </summary>
class CompanionAdjustItem
{
public:

	/// <summary>
	/// 調整項目をロードする
	/// </summary>
	void LoadItems();

	/// <summary>
	/// 調整項目を触れるようにして更新する
	/// </summary>
	void Editor();

	/// <summary>
	/// 調整項目のゲッター
	/// </summary>
	const CompanionMainData& GetMainData() const { return mainData_; }

private:

	// Jsonファイルを読み込むローダー
	JsonFunction mainJson_;

	// フィールドオブジェクト全体の項目
	CompanionMainData mainData_;

};

