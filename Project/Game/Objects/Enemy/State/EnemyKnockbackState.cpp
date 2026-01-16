#include "EnemyKnockbackState.h"

#include "Easing.h"
#include "DeltaTimer.h"

#include "Objects/Enemy/Base/BaseEnemy.h"
#include "Objects/Enemy/State/EnemyMoveState.h"

EnemyKnockbackState::EnemyKnockbackState(BaseEnemy* enemy) : EnemyBaseState(enemy) {}

void EnemyKnockbackState::Init()
{
	const float knockbackTime = 0.5f;
	const float knockbackSpeed = 3.0f;
	Vector3 velocity = enemy_->GetVelocity() * knockbackSpeed * knockbackTime;
	velocity.y = 0.0f;

	// 最初と最後の座標を作成する
	startPosition_ = enemy_->GetTransform().translation_;
	endPosition_ = enemy_->GetTransform().translation_ + velocity;
}

void EnemyKnockbackState::Finalize()
{
}

void EnemyKnockbackState::Update()
{
	const float knockbackTime = 0.5f;

	time_ += DeltaTimer::GetDeltaTime();
	float t = time_ / knockbackTime;
	t = std::clamp(t, 0.0f, 1.0f);
	t = Easing::EaseOutQuint(t);

	Vector3 translate = Vector3::Lerp(startPosition_, endPosition_, t);
	enemy_->SetTransformTranslation(translate);

	if (t >= 1.0f) {
		enemy_->ChangeState(std::make_unique<EnemyMoveState>(enemy_));
	}
}

void EnemyKnockbackState::Draw()
{
}


