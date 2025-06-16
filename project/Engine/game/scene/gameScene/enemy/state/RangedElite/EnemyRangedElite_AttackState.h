#pragma once

#include "../EnemyBaseState.h"

class EnemyRangedElite_AttackState : public EnemyBaseState
{
public:

	EnemyRangedElite_AttackState(Enemy* enemy);

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;
};

