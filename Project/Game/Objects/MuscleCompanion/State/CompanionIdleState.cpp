#include "CompanionIdleState.h"

#include <memory>

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
}

void CompanionIdleState::Update()
{
	// 集合要求がなければ何もしない
	if(!companion_->GetGatherRequested()) {
		// 距離が近づいたら待機ステートに遷移する 
		if (!companion_->SearchDistance()) {
			companion_->SetGatherRequested(true);
			companion_->SetReturnOriginal(true);
			companion_->ChangeState(std::make_unique<CompanionIdleState>(companion_));
		}
		return;
	}

	// 距離が離れたら移動ステートに遷移する
	if (companion_->SearchDistance()) {
		companion_->ChangeState(std::make_unique<CompanionMoveState>(companion_));
		return;
	}

	// プレイヤーが指示を出したら攻撃ステートに遷移する
	if (companion_->GetPlayer()->GetShot()->GetIsShot()) {
		companion_->GetPlayer()->GetShot()->SetIsShot(false);
		companion_->ChangeState(std::make_unique<CompanionDashState>(companion_));
		return;
	}
}

void CompanionIdleState::Draw()
{
}
