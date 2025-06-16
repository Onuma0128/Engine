#pragma once

#include "../EnemyBaseState.h"

class EnemyRanged_AttackState : public EnemyBaseState
{
public:

	EnemyRanged_AttackState(Enemy* enemy);

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;
};

