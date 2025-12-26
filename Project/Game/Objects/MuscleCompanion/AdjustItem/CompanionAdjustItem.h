#pragma once

#include "JsonFunction.h"

// 仲間全体の調整項目
struct CompanionMainData {
	int maxHP;						// 最大体力
	float speed;					// 移動速度
	float hitPushSpeed;				// 仲間同士の押し出しスピード
	float colliderSize;				// 仲間本体のコライダーサイズ
	Vector3 colliderOffset;			// 仲間本体のコライダー座標

	float searchUpdateTime;			// 仲間の探索を更新する時間
	float searchCancelDistance;		// 仲間の探索をやめる距離
	bool debugSpline;				// 仲間の探索経路デバッグ用
};

// 仲間のダッシュの調整項目
struct CompanionDashData {
	float dashSpeed;				// 攻撃時の移動速度
	float dashColliderScale;		// 攻撃時のコライダースケール倍率
};

// 仲間の攻撃の調整項目
struct CompanionAttackData {
	float attackColliderSize;		// 攻撃時のコライダーサイズ
	Vector3 attackColliderOffset;	// 攻撃時のコライダー座標

	float firstDashTime;			// 最初のダッシュした後の硬直時間
	float attackStartupTime;		// 攻撃を開始する硬直時間
	float attackActiveTime;			// 攻撃を行っている硬直時間
	float attackRecoveryTime;		// 攻撃が終わってから硬直時間
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
	const CompanionDashData& GetDashData() const { return dashData_; }
	const CompanionAttackData& GetAttackData() const { return attackData_; }

private:

	// Jsonファイルを読み込むローダー
	JsonFunction mainJson_;
	JsonFunction dashJson_;
	JsonFunction attackJson_;

	// 仲間全体の項目
	CompanionMainData mainData_;

	// 仲間のダッシュの項目
	CompanionDashData dashData_;

	// 仲間の攻撃の項目
	CompanionAttackData attackData_;

};

