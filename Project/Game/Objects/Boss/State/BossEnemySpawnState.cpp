#include "BossEnemySpawnState.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Boss/State/BossJumpAttackState.h"
#include "Objects/Enemy/Spawner/EnemySpawnerFactory.h"

BossEnemySpawnState::BossEnemySpawnState(BossEnemy* boss) : BossBaseState(boss) {}

void BossEnemySpawnState::Init()
{
	const auto& data = boss_->GetItems()->GetSpawnData();

	for (const auto& pos : data.positions) {
		Vector3 position = pos.Transform(Quaternion::MakeRotateMatrix(boss_->GetTransform().rotation_));
		position += boss_->GetTransform().translation_;
		boss_->GetSpawnerFactory()->SetSpawnEnemy(EnemyType::kMelee, position);
	}
}

void BossEnemySpawnState::Finalize()
{
}

void BossEnemySpawnState::Update()
{
	boss_->ChangeState(std::make_unique<BossJumpAttackState>(boss_));
}

void BossEnemySpawnState::Draw()
{
}
