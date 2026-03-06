#include "CompanionPushUpIdleState.h"

#include <memory>

#include "DeltaTimer.h"

#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "Objects/MuscleCompanion/State/CompanionIdleState.h"
#include "Objects/MuscleCompanion/State/CompanionMoveState.h"
#include "Objects/MuscleCompanion/State/CompanionDashState.h"

CompanionPushUpIdleState::CompanionPushUpIdleState(MuscleCompanion* companion) : CompanionBaseState(companion) {}

void CompanionPushUpIdleState::Init()
{
	companion_->PlayByName("Training");
}

void CompanionPushUpIdleState::Finalize()
{
	// データを取得する
	const auto& data = companion_->GetItems()->GetDashData();

	// タイムが過ぎていれば攻撃力を上げるコライダーを設定する
	if (pushUpTime_ > data.pushUpTime) {
		companion_->SetColliderName("BlowDashMuscleCompanion");
	}
}

void CompanionPushUpIdleState::Update()
{
	// データを取得する
	const auto& data = companion_->GetItems()->GetDashData();

	// 集合要求がなければ何もしない
	if (!companion_->GetGatherRequested()) {
		// 距離が近づいたら待機ステートに遷移する 
		if (!companion_->SearchDistance()) {
			companion_->SetGatherRequested(true);
			companion_->SetReturnOriginal(true);
			companion_->GetEffect()->DamageUpEffect(false);
			companion_->ChangeState(std::make_unique<CompanionIdleState>(companion_));
		} else {
			pushUpTime_ += DeltaTimer::GetDeltaTime();
			if (pushUpTime_ > data.pushUpTime) {
				companion_->GetEffect()->DamageUpEffect(true);
				if (!isPowerUp_) {
					isPowerUp_ = true;
					const auto& volume = companion_->GetItems()->GetSeVolumeData();
					companion_->GetAudio()->SoundPlayWave("MattyoPowerUp.wav", volume.powerUp);
				}
			}
		}
		return;
	}

	// 距離が離れたら移動ステートに遷移する
	if (companion_->SearchDistance()) {
		companion_->ChangeState(std::make_unique<CompanionMoveState>(companion_));
		return;
	}
}

void CompanionPushUpIdleState::Draw()
{
}
