#pragma once

#include "BossBaseState.h"

// 近接攻撃時のステート
enum class MeleeAttackState
{
	StartupTime,	// 攻撃開始までの硬直
	Attack,			// 攻撃している時間
	LandingRecover,	// 着地後の硬直
	Finish,			// 終了
};

class BossMeleeState : public BossBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="boss"></param>
	BossMeleeState(BossEnemy* boss);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	BossState GetState() const override { return BossState::Melee; }

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

	void ChangeAttackState(MeleeAttackState newState);

private:

	// 攻撃タイマー
	float timer_ = 0.0f;

	// 攻撃ステート
	MeleeAttackState meleeAttackState_ = MeleeAttackState::StartupTime;


};

