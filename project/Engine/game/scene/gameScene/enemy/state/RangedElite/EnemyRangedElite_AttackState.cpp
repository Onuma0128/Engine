#include "EnemyRangedElite_AttackState.h"

#include <memory>

#include "DeltaTimer.h"

#include "gameScene/enemy/enemy.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"
#include "../EnemyMoveState.h"

EnemyRangedElite_AttackState::EnemyRangedElite_AttackState(Enemy* enemy) :EnemyBaseState(enemy) {}

void EnemyRangedElite_AttackState::Init()
{
	chengeStateTime_ = 0.0f;
}

void EnemyRangedElite_AttackState::Finalize()
{
}

void EnemyRangedElite_AttackState::Update()
{
	// 近接攻撃のデータを取得
	RangedEliteData data = enemy_->GetItem()->GetRangedEliteData();

	chengeStateTime_ += DeltaTimer::GetDeltaTime();

	// 攻撃を始める硬直時間
	float time = data.tempData.attackStartupTime;
	if (time < chengeStateTime_) {
		// 攻撃を行っている時間
		time += data.tempData.attackActiveTime;
		if (time < chengeStateTime_) {
			// 攻撃が終わってからの硬直時間
			time += data.tempData.attackRecoveryTime;
			if (time < chengeStateTime_) {
				enemy_->ChengeState(std::make_unique<EnemyMoveState>(enemy_));
			}
		}
	}
}

void EnemyRangedElite_AttackState::Draw()
{
}
