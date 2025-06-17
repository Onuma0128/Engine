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

	// 敵のタイプごとのステートに遷移する
	void TypeChengeAttackState();

};

