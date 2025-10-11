#pragma once

#include "objects/enemy/state/EnemyBaseState.h"

class EnemyMelee_AttackState : public EnemyBaseState
{
public:

	EnemyMelee_AttackState(BaseEnemy* enemy);

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:

	float chengeStateTime_ = 0.0f;
	bool chengeAniamtion_ = false;

};

