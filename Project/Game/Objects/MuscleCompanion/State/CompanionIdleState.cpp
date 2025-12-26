#include "CompanionIdleState.h"

#include <memory>

#include "Objects/Player/Player.h"
#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "Objects/MuscleCompanion/State/CompanionMoveState.h"
#include "Objects/MuscleCompanion/State/CompanionDashState.h"

CompanionIdleState::CompanionIdleState(MuscleCompanion* companion) : CompanionBaseState(companion) {}

void CompanionIdleState::Init()
{
	companion_->PlayByName("Idle");
	companion_->GetMaterial().outlineColor = Vector3::ExprZero;
}

void CompanionIdleState::Finalize()
{
}

void CompanionIdleState::Update()
{
	if(!companion_->GetGatherRequested()) {
		return;
	}

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
