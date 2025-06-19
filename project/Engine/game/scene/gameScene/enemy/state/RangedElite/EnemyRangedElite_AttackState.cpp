#include "EnemyRangedElite_AttackState.h"

#include <memory>
#include <numbers>

#include "DeltaTimer.h"

#include "gameScene/enemy/enemy.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"
#include "../EnemyMoveState.h"

EnemyRangedElite_AttackState::EnemyRangedElite_AttackState(Enemy* enemy) :EnemyBaseState(enemy) {}

void EnemyRangedElite_AttackState::Init()
{
	chengeStateTime_ = 0.0f;

	isAttack_ = false;
	
	enemy_->GetEffect()->SetBulletPredictionEffect(true);
}

void EnemyRangedElite_AttackState::Finalize()
{
	enemy_->GetEffect()->SetBulletPredictionEffect(false);
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
		Attack();
		if (time < chengeStateTime_) {
			// 攻撃が終わってからの硬直時間
			time += data.tempData.attackRecoveryTime;
			enemy_->GetEffect()->SetBulletPredictionEffect(false);
			if (time < chengeStateTime_) {
				enemy_->ChengeState(std::make_unique<EnemyMoveState>(enemy_));
				return;
			}
		}
	}
}

void EnemyRangedElite_AttackState::Draw()
{
}

void EnemyRangedElite_AttackState::Attack()
{
	if (isAttack_) { return; }

	// 近接攻撃のデータを取得
	RangedEliteData data = enemy_->GetItem()->GetRangedEliteData();

	float rad = -data.bulletRadSpace;
	float pi = std::numbers::pi_v<float> / 4.0f;
	for (auto& bullet : enemy_->GetBullets()) {
		WorldTransform transform = enemy_->GetTransform();
		Quaternion quaternion = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, pi * rad);
		transform.rotation_ = transform.rotation_ * quaternion;
		bullet->Attack(transform);
		rad += data.bulletRadSpace;
	}
	isAttack_ = true;
}