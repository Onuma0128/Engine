#include "CompanionShieldKnockbackState.h"

#include "DeltaTimer.h"
#include "Easing.h"

#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "Objects/MuscleCompanion/AdjustItem/CompanionAdjustItem.h"
#include "Objects/MuscleCompanion/State/CompanionIdleState.h"

CompanionShieldKnockbackState::CompanionShieldKnockbackState(MuscleCompanion* companion) :CompanionBaseState(companion) {}

void CompanionShieldKnockbackState::Init()
{
	// アニメーションの初期化
	companion_->ForcePlayByName("Damage", 0.0f);
	companion_->SetAnimationTime(0.0f);
	companion_->GetTimeStop() = true;
	companion_->SetReturnOriginal(false);
	companion_->SetGatherRequested(false);

	// データを取得する
	const auto& data = companion_->GetItems()->GetKnockbackData();
	float speed = data.shieldKnockbackSpeed;
	maxTime_ = data.shieldKnockbackTime;

	// ノックバック方向を取得する
	Vector3 direction = data.shieldKnockbackDire;
	// 盾が仲間の右にあるか判定
	Vector3 localPosition = Vector3(companion_->GetKnockbackPos()).Transform(Matrix4x4::Inverse(companion_->GetTransform().matWorld_));
	if (0.0f <= localPosition.x) {
		direction.x = -direction.x;
	}

	// ノックバック方向を取得する
	direction.y = 0.0f;
	if (direction.Length() != 0.0f) { direction = direction.Normalize(); }
	direction = direction.Transform(Quaternion::MakeRotateMatrix(companion_->GetKnockbackRotate()));

	// ノックバックされた座標を計算する
	prePos_ = companion_->GetTransform().translation_;
	target_ = prePos_ + direction * speed * maxTime_;
}

void CompanionShieldKnockbackState::Finalize()
{
	companion_->GetTimeStop() = false;
}

void CompanionShieldKnockbackState::Update()
{
	// 時間を更新する
	timer_ += DeltaTimer::GetDeltaTime();
	timer_ = std::clamp(timer_, 0.0f, maxTime_);
	float t = Easing::EaseOutQuint(timer_ / maxTime_);
	Vector3 position = Vector3::Lerp(prePos_, target_, t);
	companion_->SetTransformTranslation(position);

	if (timer_ >= maxTime_) {
		companion_->ChangeState(std::make_unique<CompanionIdleState>(companion_));
	}
}

void CompanionShieldKnockbackState::Draw()
{
}
