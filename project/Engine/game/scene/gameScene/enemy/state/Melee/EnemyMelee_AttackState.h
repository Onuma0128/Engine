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

	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

private:

	float chengeStateTime_ = 0.0f;

};

