#pragma once

#include <vector>

#include "JsonFunction.h"

// ボス全体の調整項目
struct BossMainData {
	int maxHP = 20;					// ボスの最大体力
	float speed = 2.0f;				// ボスの移動速度
	float rayDistance;				// ボスの視線距離
	Vector3 rayOffset;				// ボスの視線座標

	float colliderSize;				// ボスのコライダーサイズ
	Vector3 colliderOffset;			// ボスのコライダー座標

	float searchUpdateTime;			// ボスの探索を更新する時間
	bool debugSpline = false;		// ボスの経路探索のデバッグ表示
};

// ボスの敵スポーン時の調整項目
struct BossEnemySpawnData {
	int maxSpawnCount;				// ボスの敵をスポーンさせる最大数
	std::vector<Vector3> positions;	// ボスの敵をスポーンさせる座標
};

// ボスの近接攻撃の調整項目
struct BossMeleeData {
	float attackColliderSize;		// 攻撃時のコライダーサイズ
	Vector3 attackColliderOffset;	// 攻撃時のコライダー座標

	float attackStartupTime;		// 攻撃を開始する硬直時間
	float attackTime;				// 攻撃している時間
	float attackRecoveryTime;		// 攻撃が終わってから硬直時間
};

// ボスのジャンプ攻撃の調整項目
struct BossJumpAttackData {
	float airSpeed;					// 空中にいる時のスピード
	float jumpVelocityY;			// ジャンプの速度
	float jumpAccelerY;				// ジャンプの加速度

	float attackColliderSize;		// 攻撃時のコライダーサイズ
	Vector3 attackColliderOffset;	// 攻撃時のコライダー座標

	float attackStartupTime;		// 攻撃を開始する硬直時間
	float JumpUpTime;				// ジャンプをする時間
	float airHoldTime;				// 空中にいる時間
	float fallDownTime;				// 落下にかかる時間
	float attackRecoveryTime;		// 攻撃が終わってから硬直時間
};

// ボスのダッシュ攻撃の調整項目
struct BossDashAttackData {
	float dashSpeed;				// ダッシュ時のスピード
	float attackColliderSize;		// 攻撃時のコライダーサイズ
	Vector3 attackColliderOffset;	// 攻撃時のコライダー座標

	float attackStartupTime;		// 攻撃を開始する硬直時間
	float dashTime;					// ダッシュをしている時間
	float attackRecoveryTime;		// 攻撃が終わってから硬直時間
};

/// <summary>
/// ボスの調整項目を調整、ロード、保存管理するクラス
/// </summary>
class BossAdjustItem
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
	const BossMainData& GetMainData() const { return mainData_; }
	const BossEnemySpawnData& GetSpawnData()const { return spawnData_; }
	const BossMeleeData& GetMeleeData()const { return meleeData_; }
	const BossJumpAttackData& GetJumpAttackData() const { return jumpAttackData_; }
	const BossDashAttackData& GetDashAttackData() const { return dashAttackData_; }

private:

	// Jsonファイルを読み込むローダー
	JsonFunction mainJson_;
	JsonFunction spawnJson_;
	JsonFunction meleeAttackJson_;
	JsonFunction jumpAttackJson_;
	JsonFunction dashAttackJson_;

	BossMainData mainData_;
	BossEnemySpawnData spawnData_;
	BossMeleeData meleeData_;
	BossJumpAttackData jumpAttackData_;
	BossDashAttackData dashAttackData_;
	int savedSpawnCount_ = 0;

};

