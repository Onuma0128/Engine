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
		companion_->PlayByName("Wave");
	} else {
		companion_->PlayByName("Idle");
	}
	companion_->GetMaterial().outlineColor = Vector3::ExprZero;
}

void CompanionIdleState::Finalize()
{
}

void CompanionIdleState::Update()
{
	// 集合要求がなければ何もしない
	if(!companion_->GetGatherRequested()) {
		return;
	}

	// 距離が離れたら移動ステートに遷移する
	if (companion_->SearchDistance()) {
		companion_->ChangeState(std::make_unique<CompanionMoveState>(companion_));
		return;
	}

	// プレイヤーが指示を出したら攻撃ステートに遷移する
	if (companion_->GetPlayer()->GetShot()->GetIsShot()) {
		companion_->ChangeState(std::make_unique<CompanionDashState>(companion_));
		companion_->GetPlayer()->GetShot()->SetIsShot(false);
		return;
	}
}

void CompanionIdleState::Draw()
{
}
