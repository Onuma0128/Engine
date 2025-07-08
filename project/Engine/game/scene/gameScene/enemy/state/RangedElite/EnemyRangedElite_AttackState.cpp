#include "EnemyRangedElite_AttackState.h"

#include <memory>
#include <numbers>

#include "DeltaTimer.h"

#include "gameScene/player/Player.h"
#include "gameScene/enemy/Enemy.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"
#include "../EnemyMoveState.h"

EnemyRangedElite_AttackState::EnemyRangedElite_AttackState(Enemy* enemy) :EnemyBaseState(enemy) {}

void EnemyRangedElite_AttackState::Init()
{
	chengeStateTime_ = 0.0f;

	isAttack_ = false;
	
	enemy_->GetEffect()->SetBulletPredictionEffect(true);

	enemy_->PlayByName("Idle_Gun");
}

void EnemyRangedElite_AttackState::Finalize()
{
	enemy_->GetEffect()->SetBulletPredictionEffect(false);

	//enemy_->PlayByName("Run");
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
	} else {
		// 時間ギリギリまで追従する
		Vector3 velocity = Vector3(enemy_->GetPlayer()->GetTransform().translation_ - enemy_->GetTransform().translation_);
		velocity.y = 0.0f;
		if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }
		enemy_->SetVelocity(velocity);

		// 移動時の回転の処理
		if (velocity.Length() != 0.0f) {
			Vector3 foward = Vector3::ExprUnitZ;
			Vector3 targetDir = Vector3{ -velocity.x,0.0f,velocity.z };

			// velocityから回転を求める
			Matrix4x4 targetMatrix = Matrix4x4::DirectionToDirection(foward, targetDir);
			Quaternion targetRotation = Quaternion::FormRotationMatrix(targetMatrix);
			Quaternion currentRotation = enemy_->GetTransform().rotation_;
			Quaternion result = Quaternion::Slerp(currentRotation, targetRotation, 0.5f);

			// 回転を適応
			enemy_->GetTransform().rotation_ = result;
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