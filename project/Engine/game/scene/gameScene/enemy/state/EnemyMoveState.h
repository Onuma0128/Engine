#pragma once

#include "EnemyBaseState.h"

class EnemyMoveState : public EnemyBaseState
{
public:

	EnemyMoveState(Enemy* enemy);

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:

	// 敵のタイプごとのスピードを取得する
	const float GetTypeSpeed();

	// 敵のタイプごとの攻撃ステートへの距離を取得する
	const float GetTypeAttackDistance();

	// 敵のタイプごとの攻撃クールタイムを取得する
	const float GetTypeAttackCoolTime();

	// 敵のタイプごとのステートに遷移する
	void TypeChengeAttackState();

	// 敵のタイプごとのアニメーションを用意する
	void MoveAnimation();
	void AttackCoolTimeAnimation();


private:

	float attackCoolTime_ = 0.0f;

	bool isMoveAnima_ = false;
	bool isIdleAnima_ = false;

	bool inAttackRange_ = false;

};

