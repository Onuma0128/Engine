#pragma once

#include "objects/enemy/state/EnemyBaseState.h"

class EnemyRanged_AttackState : public EnemyBaseState
{
public:

	EnemyRanged_AttackState(BaseEnemy* enemy);

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:

	void Attack();

	void LookOn();

private:

	float chengeStateTime_ = 0.0f;

	bool isAttack_ = false;
	bool chengeAniamtion_ = false;

};

