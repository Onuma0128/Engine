#pragma once

#include "../EnemyBaseState.h"

class EnemyShieldBearer_AttackState : public EnemyBaseState
{
public:

	EnemyShieldBearer_AttackState(Enemy* enemy);

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;
};

