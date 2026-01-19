#include "CompanionIdleState.h"

#include <memory>

#include "DeltaTimer.h"

#include "Objects/Player/Player.h"
#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "Objects/MuscleCompanion/State/CompanionMoveState.h"
#include "Objects/MuscleCompanion/State/CompanionDashState.h"

CompanionIdleState::CompanionIdleState(MuscleCompanion* companion) : CompanionBaseState(companion) {}

void CompanionIdleState::Init()
{
	if (!companion_->GetGatherRequested()) {
		companion_->PlayByName("Training");
	} else {
		companion_->PlayByName("Wait");
	}
}

void CompanionIdleState::Finalize()
{
	// データを取得する
	const auto& data = companion_->GetItems()->GetDashData();

	// タイムが過ぎていれば攻撃力を上げるコライダーを設定する
	if (pushUpTime_ > data.pushUpTime) {
		companion_->SetColliderName("BlowDashMuscleCompanion");
	}
}

void CompanionIdleState::Update()
{
	// データを取得する
	const auto& data = companion_->GetItems()->GetDashData();

	// 集合要求がなければ何もしない
	if(!companion_->GetGatherRequested()) {
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

	// プレイヤーが指示を出したら攻撃ステートに遷移する
	if (companion_->GetDashAttack()) {
		const auto& volume = companion_->GetItems()->GetSeVolumeData();
		companion_->GetAudio()->SoundPlayWave("MattyoShot.wav", volume.shot);
		companion_->GetPlayer()->GetShot()->SetIsShot(false);
		companion_->SetDashAttack(false);
		companion_->ChangeState(std::make_unique<CompanionDashState>(companion_));
		return;
	}
}

void CompanionIdleState::Draw()
{
}
