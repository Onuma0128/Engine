#include "EnemyShieldBearer_AttackState.h"

#include <memory>

#include "Collider.h"
#include "DeltaTimer.h"

#include "gameScene/enemy/enemy.h"
#include "gameScene/enemy/weapon/EnemyWeaponBase.h"
#include "gameScene/enemy/weapon/axe/EnemyAxe.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"
#include "../EnemyMoveState.h"

EnemyShieldBearer_AttackState::EnemyShieldBearer_AttackState(Enemy* enemy) : EnemyBaseState(enemy) {}

void EnemyShieldBearer_AttackState::Init()
{
	chengeStateTime_ = 0.0f;
}

void EnemyShieldBearer_AttackState::Finalize()
{
	enemy_->PlayByName("Run_Arms");
	enemy_->GetWeapon()->SetIsActive(false);
	enemy_->GetWeapon()->Update();
}

void EnemyShieldBearer_AttackState::Update()
{
	// 近接攻撃のデータを取得
	ShieldBearerData data = enemy_->GetItem()->GetShieldBearerData();
	EnemyWeaponBase* weapon = enemy_->GetWeapon();

	chengeStateTime_ += DeltaTimer::GetDeltaTime();

	// 攻撃を始める硬直時間
	float time = data.tempData.attackStartupTime;
	weapon->SetIsActive(false);
	if (time < chengeStateTime_) {
		// 攻撃を行っている時間
		time += data.tempData.attackActiveTime;
		weapon->SetIsActive(true);
		if (time < chengeStateTime_) {
			// 攻撃が終わってからの硬直時間
			time += data.tempData.attackRecoveryTime;
			weapon->SetIsActive(false);
			if (time < chengeStateTime_) {
				enemy_->ChengeState(std::make_unique<EnemyMoveState>(enemy_));
				return;
			} else {
				if (!chengeAniamtion_) {
					chengeAniamtion_ = true;
					enemy_->PlayByName("Idle");
				}
			}
		} else {
			if (chengeAniamtion_) {
				chengeAniamtion_ = false;
				enemy_->PlayByName("Idle_Attack");
			}
		}
	} else {
		if (!chengeAniamtion_) {
			chengeAniamtion_ = true;
			enemy_->PlayByName("Idle");
		}
	}
}

void EnemyShieldBearer_AttackState::Draw()
{
}
