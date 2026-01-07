#include "BossEnemySpawnState.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Boss/State/BossJumpAttackState.h"
#include "Objects/Boss/State/BossMoveState.h"

#include "Objects/Enemy/Spawner/EnemySpawnerFactory.h"

BossEnemySpawnState::BossEnemySpawnState(BossEnemy* boss) : BossBaseState(boss) {}

void BossEnemySpawnState::Init()
{
	// データを取得
	const auto& data = boss_->GetItems()->GetSpawnData();
	const auto& volume = boss_->GetItems()->GetSeVolumeData();
	// 効果音を鳴らす
	boss_->GetAudio()->SoundPlayWave("BossEnemySpawn.wav", volume.enemySpawn);

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
	boss_->ChangeState(std::make_unique<BossMoveState>(boss_));
}

void BossEnemySpawnState::Draw()
{
}
