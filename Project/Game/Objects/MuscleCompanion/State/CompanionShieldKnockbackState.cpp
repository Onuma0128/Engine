
#include "CompanionShieldKnockbackState.h"

#include "DeltaTimer.h"
#include "Easing.h"

#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "Objects/MuscleCompanion/AdjustItem/CompanionAdjustItem.h"
#include "Objects/MuscleCompanion/State/CompanionMoveState.h"

CompanionShieldKnockbackState::CompanionShieldKnockbackState(MuscleCompanion* companion)
    : CompanionBaseState(companion)
{
}

void CompanionShieldKnockbackState::Init()
{
    // アニメーションの初期化
    companion_->ForcePlayByName("Damage", 0.0f);
    companion_->SetAnimationTime(0.0f);
    companion_->GetTimeStop() = true;

    // データを取得する
    const auto& data = companion_->GetItems()->GetKnockbackData();
    float speed = data.shieldKnockbackSpeed;
    maxTime_ = data.shieldKnockbackTime;

    // ノックバックされた座標を計算する
    prePos_ = companion_->GetTransform().translation_;
    NumaEngine::Vector3 dir = data.shieldKnockbackDire;
    dir.y = 0.0f;
    if (dir.Length() != 0.0f) { dir = dir.Normalize(); }
    target_ = prePos_ + dir * speed * maxTime_;
}

void CompanionShieldKnockbackState::Finalize()
{
    companion_->SetGatherRequested(true);
    companion_->GetTimeStop() = false;
}

void CompanionShieldKnockbackState::Update()
{
    // 時間を更新する
    timer_ += DeltaTimer::GetDeltaTime();
    timer_ = std::clamp(timer_, 0.0f, maxTime_);
    float t = Easing::EaseOutQuint(timer_ / maxTime_);
    NumaEngine::Vector3 position = NumaEngine::Vector3::Lerp(prePos_, target_, t);
    companion_->SetTransformTranslation(position);

    if (timer_ >= maxTime_) {
        companion_->ChangeState(std::make_unique<CompanionMoveState>(companion_));
    }
}

void CompanionShieldKnockbackState::Draw()
{
}

