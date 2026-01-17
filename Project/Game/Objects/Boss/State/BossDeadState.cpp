#include "BossDeadState.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Enemy/Spawner/EnemySpawnerFactory.h"

BossDeadState::BossDeadState(BossEnemy* boss) : BossBaseState(boss) {}

void BossDeadState::Init()
{
	boss_->PlayByName("Death");
	boss_->GetTimeStop() = true;
	boss_->SetAnimationTime(0.0f);
	boss_->GetSpawnerFactory()->Reset();
}

void BossDeadState::Finalize()
{
}

void BossDeadState::Update()
{
}

void BossDeadState::Draw()
{
}
