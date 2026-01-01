#include "CompanionKnockbackState.h"

#include "DeltaTimer.h"
#include "Easing.h"

#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "Objects/MuscleCompanion/AdjustItem/CompanionAdjustItem.h"
#include "Objects/MuscleCompanion/State/CompanionMoveState.h"

CompanionKnockbackState::CompanionKnockbackState(MuscleCompanion* companion) : CompanionBaseState(companion) {}

void CompanionKnockbackState::Init()
{
	// アニメーションの初期化
	companion_->PlayByName("No");

	// ノックバック方向を取得する
	Vector3 direction = companion_->GetKnockbackDire();
	direction.y = 0.0f;
	if (direction.Length() != 0.0f) { direction = direction.Normalize(); }

	// データを取得する
	const auto& data = companion_->GetItems()->GetKnockbackData();
	float speed = data.knockbackSpeed;
	maxTime_ = data.knockbackTime;

	// ノックバックされた座標を計算する
	prePos_ = companion_->GetTransform().translation_;
	target_ = prePos_ +	direction * speed * maxTime_;
}

void CompanionKnockbackState::Finalize()
{
	companion_->SetGatherRequested(true);
}

void CompanionKnockbackState::Update()
{
	// 時間を更新する
	timer_ += DeltaTimer::GetDeltaTime();
	timer_ = std::clamp(timer_, 0.0f, maxTime_);
	float t = Easing::EaseOutQuint(timer_ / maxTime_);
	Vector3 position = Vector3::Lerp(prePos_, target_, t);
	companion_->SetTransformTranslation(position);

	if (timer_ >= maxTime_) {
		companion_->ChangeState(std::make_unique<CompanionMoveState>(companion_));
	}
}

void CompanionKnockbackState::Draw()
{
}
