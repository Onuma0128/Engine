#pragma once

#include <memory>

#include "EnemyBaseState.h"

/// <summary>
/// 敵の移動ステートクラス
/// </summary>
class EnemyMoveState : public EnemyBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyMoveState(BaseEnemy* enemy);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 終了処理
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

	/// <summary>
	/// 敵のタイプごとのスピードを取得する
	/// </summary>
	/// <returns></returns>
	const float GetTypeSpeed();

	/// <summary>
	/// 敵のタイプごとの攻撃ステートへの距離を取得する
	/// </summary>
	/// <returns></returns>
	const float GetTypeAttackDistance();

	/// <summary>
	/// 敵のタイプごとの攻撃クールタイムを取得する
	/// </summary>
	/// <returns></returns>
	const float GetTypeAttackCoolTime();

	/// <summary>
	/// 敵のタイプごとの攻撃ステートに遷移する
	/// </summary>
	void TypeChengeAttackState();

	/// <summary>
	/// 移動アニメーションを再生する
	/// </summary>
	void MoveAnimation();

	/// <summary>
	/// 攻撃クールタイムアニメーションを再生する
	/// </summary>
	void AttackCoolTimeAnimation();


private:

	float searchTime_ = 0.0f;

	float attackCoolTime_ = 0.0f;

	bool isMoveAnima_ = false;
	bool isIdleAnima_ = false;

	bool inAttackRange_ = false;

};

