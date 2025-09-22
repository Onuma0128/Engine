#pragma once

#include "../EnemyBaseState.h"

class EnemyRangedElite_AttackState : public EnemyBaseState
{
public:

	EnemyRangedElite_AttackState(BaseEnemy* enemy);

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:

	void Attack();

private:

	float chengeStateTime_ = 0.0f;

	bool isAttack_ = false;

};

