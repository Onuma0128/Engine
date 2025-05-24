#include "EnemyDeadState.h"

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
}

void EnemyDeadState::Init()
{
	GlobalInit();

	// 死亡するフレーム
	deadFrame_ = maxDeadFrame_;
	// 敵がノックバックする方向
	velocity_ = enemy_->GetVelocity();
	// ノックバックする距離
	target_ = enemy_->GetTransform().translation_ + velocity_ * 3.0f;
	// デフォルトのScaleを取得
	defaultScale_ = enemy_->GetTransform().scale_;
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
		Vector3 target = Vector3::Lerp(enemyPosition, target_, t);
		enemy_->GetTransform().translation_ = target;

	// それ以降に死亡時パーティクルを出す
	} else if (deadFrame_ > (maxDeadFrame_ - knockbackFrame_ - particleFrame_)) {
		enemy_->GetEffect()->SetDeadEffect(true);
	} else {
		enemy_->GetEffect()->SetDeadEffect(false);
	}

	// 5秒経ったら消す
	if (deadFrame_ <= 0.0f) {
		enemy_->SetIsDead(true);
	}
}

void EnemyDeadState::Draw()
{
}
