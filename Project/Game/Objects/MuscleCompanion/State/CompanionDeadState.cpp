#include "CompanionDeadState.h"

#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"

CompanionDeadState::CompanionDeadState(MuscleCompanion* companion) : CompanionBaseState(companion) {}

void CompanionDeadState::Init()
{
	companion_->ForcePlayByName("Dead", 0.0f);
	companion_->SetAnimationTime(0.0f);
	companion_->GetTimeStop() = true;

	companion_->SetColliderIsActive(false);
	companion_->GetAttackCollider()->SetActive(false);
	companion_->GetFollowerCollider()->SetActive(false);
	companion_->GetEffect()->DamageUpEffect(false);
}

void CompanionDeadState::Finalize()
{
}

void CompanionDeadState::Update()
{
}

void CompanionDeadState::Draw()
{
}
