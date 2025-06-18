#include "EnemyRanged_AttackState.h"

#include <memory>

#include "DeltaTimer.h"

#include "gameScene/enemy/enemy.h"
#include "gameScene/enemy/bullet/EnemyBullet.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"
#include "../EnemyMoveState.h"

EnemyRanged_AttackState::EnemyRanged_AttackState(Enemy* enemy) :EnemyBaseState(enemy) {}

void EnemyRanged_AttackState::Init()
{
	chengeStateTime_ = 0.0f;

	isAttack_ = false;
}

void EnemyRanged_AttackState::Finalize()
{
}

void EnemyRanged_AttackState::Update()
{
	// 近接攻撃のデータを取得
	RangedData data = enemy_->GetItem()->GetRangedData();

	chengeStateTime_ += DeltaTimer::GetDeltaTime();

	// 攻撃を始める硬直時間
	float time = data.tempData.attackStartupTime;
	if (time < chengeStateTime_) {
		// 攻撃を行っている時間
		time += data.tempData.attackActiveTime;
		Attack();
		if (time < chengeStateTime_) {
			// 攻撃が終わってからの硬直時間
			time += data.tempData.attackRecoveryTime;
			if (time < chengeStateTime_) {
				enemy_->ChengeState(std::make_unique<EnemyMoveState>(enemy_));
				return;
			}
		}
	}
}

void EnemyRanged_AttackState::Draw()
{
}

void EnemyRanged_AttackState::Attack()
{
	if (isAttack_) { return; }
	for (auto& bullet : enemy_->GetBullets()) {
		bullet->Attack(enemy_->GetTransform());
	}
	isAttack_ = true;
}
