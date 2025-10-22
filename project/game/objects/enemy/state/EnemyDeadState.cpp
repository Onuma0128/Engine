#include "EnemyDeadState.h"

#include <numbers>

#include "DeltaTimer.h"
#include "Easing.h"

#include "objects/enemy/base/BaseEnemy.h"
#include "objects/enemy/weapon/EnemyWeaponBase.h"

#include "objects/enemy/adjustItem/EnemyAdjustItem.h"

EnemyDeadState::EnemyDeadState(BaseEnemy* enemy) :EnemyBaseState(enemy) {}

void EnemyDeadState::GlobalInit()
{
	const auto data = enemy_->GetItem()->GetMainData();

	// 死亡までの時間(5.0f)
	maxDeadTimer_ = data.maxDeadTimer;
	// ノックバックしている時間(1.0f)
	knockbackTimer_ = data.kNockbackTimer;
	// ノックバック時スケールが変化している時間
	kNockbackScaleTimer_ = data.kNockbackScaleTimer;
	// パーティクルが出ている時間(3.0f)
	particleTimer_ = data.particleTimer;

	// 敵が死ぬ時のvelocityとacceleration(10.0f,100.0f)
	velocityY_ = data.kNockbackVelocityY;
	accelerationY_ = data.kNockbackAccelerY;
	kNockbackScale_ = data.kNockbackScale;
}

void EnemyDeadState::Init()
{
	GlobalInit();

	// 死亡するフレーム
	deadTimer_ = maxDeadTimer_;
	// 敵がノックバックする方向
	velocity_ = enemy_->GetVelocity();
	// ノックバックの際に敵が起こすアクションの計算
	ResultTargetOffset();
	// シールドを持っているなら
	if (enemy_->GetShieldWeapon() != nullptr) { enemy_->GetShieldWeapon()->SetIsActive(false); }
}

void EnemyDeadState::Finalize()
{
}

void EnemyDeadState::Update()
{
	const auto data = enemy_->GetItem()->GetMainData();

	deadTimer_ -= DeltaTimer::GetDeltaTime();
	deadTimer_ = std::clamp(deadTimer_, 0.0f, maxDeadTimer_);
	// スケール変化を更新
	if (deadTimer_ > (maxDeadTimer_ - data.kNockbackScaleTimer)) {
		float t = std::clamp(((maxDeadTimer_ - deadTimer_) / data.kNockbackScaleTimer), 0.0f, 1.0f);
		float changeScale = std::sin(t * std::numbers::pi_v<float>) * kNockbackScale_;
		enemy_->GetTransform().scale_ = Vector3::ExprUnitXYZ + (Vector3::ExprUnitXYZ * changeScale);
	} else {
		float t = ((deadTimer_ - data.kNockbackScaleTimer) / (maxDeadTimer_ - data.kNockbackScaleTimer));
		enemy_->GetTransform().scale_ = defaultScale_ * t; 
	}

	// 最初の1秒はヒットバック
	if (deadTimer_ > (maxDeadTimer_ - knockbackTimer_)) {
		float t = std::clamp(1.0f - (deadTimer_ - 4.0f), 0.0f, 1.0f);
		Vector3 enemyPosition = enemy_->GetTransform().translation_;
		if (enemy_->GetTransform().translation_.y > 0.5f) {
			enemyPosition.y += velocity_.y * DeltaTimer::GetDeltaTime();
			target_.y += velocity_.y * DeltaTimer::GetDeltaTime();
		}
		Vector3 target = Vector3::Lerp(enemyPosition, target_, t);
		enemy_->GetTransform().translation_ = target;
		velocity_.y -= accelerationY_ * DeltaTimer::GetDeltaTime();
		// 回転をスラープさせる
		defaultRotate_ = Quaternion::Slerp(defaultRotate_, targetRotate_, 0.2f);
		enemy_->GetTransform().rotation_ = defaultRotate_;

	// それ以降に死亡時パーティクルを出す
	} else if (deadTimer_ > (maxDeadTimer_ - knockbackTimer_ - particleTimer_)) {
		enemy_->GetEffect()->SetDeadEffect(true);
	} else {
		enemy_->GetEffect()->SetDeadEffect(false);
	}

	// 5秒経ったら消す
	if (deadTimer_ <= 0.0f) {
		enemy_->SetIsDead(true);
	} else {
		if (!chengeAnimation_) {
			if (enemy_->ForcePlayByName("Death", 0.1f)) {
				enemy_->GetTimeStop() = true;
				chengeAnimation_ = true;
			}
		}
	}
}

void EnemyDeadState::Draw()
{
}

void EnemyDeadState::ResultTargetOffset()
{
	// ノックバックする座標
	target_ = enemy_->GetTransform().translation_ + velocity_ * 3.0f;
	velocity_.y = velocityY_;
	// ノックバックする回転
	// 敵のローカル空間で弾の座標が左か右にいるか計算
	isLeft_ = false;
	Vector3 localPosition = Vector3(enemy_->GetPlayerBullet()).Transform(Matrix4x4::Inverse(enemy_->GetTransform().matWorld_));
	Quaternion rotateY = Quaternion::IdentityQuaternion();
	std::mt19937 randomEngine_(seedGenerator_());
	// 弾が敵の左にあるか判定
	if (0.0f > localPosition.x) {
		isLeft_ = true;
	}
	// 左なら
	if (isLeft_) {
		std::uniform_real_distribution<float> RotateY(-std::numbers::pi_v<float> / 2.0f, 0.0f);
		rotateY = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, RotateY(randomEngine_));
	// 右なら
	} else {
		std::uniform_real_distribution<float> RotateY(0.0f, std::numbers::pi_v<float> / 2.0f);
		rotateY = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, RotateY(randomEngine_));
	}
	defaultRotate_ = enemy_->GetTransform().rotation_;
	targetRotate_ = enemy_->GetTransform().rotation_ * rotateY;

	// デフォルトのScaleを取得
	defaultScale_ = enemy_->GetTransform().scale_;
}
