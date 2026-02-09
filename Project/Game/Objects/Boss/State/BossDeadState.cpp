#include "BossDeadState.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Enemy/Spawner/EnemySpawnerFactory.h"

BossDeadState::BossDeadState(BossEnemy* boss) : BossBaseState(boss) {}

void BossDeadState::Init()
{
	boss_->ForcePlayByName("Death", 0.0f);
	boss_->SetAnimationTime(0.0f);
	boss_->GetTimeStop() = true;
	boss_->GetSpawnerFactory()->Reset();
}

void BossDeadState::Finalize()
{
}

void BossDeadState::Update()
{
	boss_->GetTimeStop() = true;
}

void BossDeadState::Draw()
{
}
