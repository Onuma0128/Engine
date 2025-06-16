#pragma once

#include "../EnemyBaseState.h"

class EnemyMelee_AttackState : public EnemyBaseState
{
public:

	EnemyMelee_AttackState(Enemy* enemy);

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;
};

