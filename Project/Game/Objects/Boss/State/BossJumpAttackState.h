#pragma once

#include "BossBaseState.h"

// ジャンプ攻撃時のステート
enum class JumpAttackState
{
	StartupTime,	// 攻撃開始までの硬直
	JumpUp,			// ジャンプ上昇
	AirHold,		// 空中滞空
	FallDown,		// 落下攻撃
	LandingRecover,	// 着地後の硬直
	Finish,			// 終了
};

/// <summary>
/// ジャンプ攻撃ステートクラス
/// </summary>
class BossJumpAttackState : public BossBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="boss"></param>
	BossJumpAttackState(BossEnemy* boss);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	BossState GetState() const override { return BossState::JumpAttack; }

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

	void UpdateJump();

	void ChangeAttackState(JumpAttackState newState);

private:

	// 攻撃タイマー
	float timer_ = 0.0f;

	// 攻撃ステート
	JumpAttackState jumpAttackState_ = JumpAttackState::StartupTime;

	// 開始Y座標
	float startY_ = 0.0f;

	// ジャンプ関連パラメータ
	float jumpVelocityY_ = 0.0f;
	float jumpAccelerY_ = 0.0f;

};

