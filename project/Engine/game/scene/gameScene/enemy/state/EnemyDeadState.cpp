#include "EnemyDeadState.h"

#include <numbers>

#include "DeltaTimer.h"
#include "Easing.h"

#include "gameScene/enemy/enemy.h"
#include "gameScene/enemy/state/EnemyMoveState.h"

EnemyDeadState::EnemyDeadState(Enemy* enemy) :EnemyBaseState(enemy) {}

void EnemyDeadState::GlobalInit()
{
	// 死亡までの時間
	maxDeadFrame_ = 5.0f;
	// ノックバックしている時間
	knockbackFrame_ = 1.0f;
	// パーティクルが出ている時間
	particleFrame_ = 3.0f;

	// 敵が死ぬ時のvelocityとacceleration
	velocityY_ = 10.0f;
	accelerationY_ = 100.0f;
}

void EnemyDeadState::Init()
{
	GlobalInit();

	// 死亡するフレーム
	deadFrame_ = maxDeadFrame_;
	// 敵がノックバックする方向
	velocity_ = enemy_->GetVelocity();
	// ノックバックの際に敵が起こすアクションの計算
	ResultTargetOffset();
}

void EnemyDeadState::Finalize()
{
}

void EnemyDeadState::Update()
{
	deadFrame_ -= DeltaTimer::GetDeltaTime();
	deadFrame_ = std::clamp(deadFrame_, 0.0f, maxDeadFrame_);
	enemy_->GetTransform().scale_ = defaultScale_ * (deadFrame_ / maxDeadFrame_);

	// 最初の1秒はヒットバック
	if (deadFrame_ > (maxDeadFrame_ - knockbackFrame_)) {
		float t = std::clamp(1.0f - (deadFrame_ - 4.0f), 0.0f, 1.0f);
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
	} else if (deadFrame_ > (maxDeadFrame_ - knockbackFrame_ - particleFrame_)) {
		enemy_->GetEffect()->SetDeadEffect(true);
	} else {
		enemy_->GetEffect()->SetDeadEffect(false);
	}

	// 5秒経ったら消す
	if (deadFrame_ <= 0.0f) {
		enemy_->SetIsDead(true);
	} else {
		if (!chengeAnimation_) {
			if (enemy_->PlayByName("Death"),0.1f) {
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
