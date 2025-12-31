#pragma once

#include "BossBaseState.h"

#include "Vector3.h"

// ダッシュ攻撃時のステート
enum class DashAttackState
{
	StartupTime,	// 攻撃開始までの硬直
	DashAttack,		// 攻撃している時間
	LandingRecover,	// 着地後の硬直
	Finish,			// 終了
};

/// <summary>
/// ダッシュ攻撃のステートクラス
/// </summary>
class BossDashAttackState : public BossBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="boss"></param>
	BossDashAttackState(BossEnemy* boss);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	BossState GetState() const override { return BossState::DashAttack; }

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

private:

	void ChangeAttackState(DashAttackState newState);

	void CreateVelocity();

private:

	// 攻撃タイマー
	float timer_ = 0.0f;

	// 攻撃ステート
	DashAttackState dashAttackState_ = DashAttackState::StartupTime;

	// 速度
	Vector3 velocity_ = Vector3::ExprUnitZ;

};

