#include "BossMeleeState.h"

#include "DeltaTimer.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Boss/State/BossMoveState.h"

BossMeleeState::BossMeleeState(BossEnemy* boss) : BossBaseState(boss) {}

void BossMeleeState::Init()
{
	boss_->PlayByName("Jump");
}

void BossMeleeState::Finalize()
{
	boss_->GetAttackCollider()->SetActive(false);
}

void BossMeleeState::Update()
{
	// データを取得する
	const auto& data = boss_->GetItems()->GetMeleeData();

	// タイムを加算
	timer_ += DeltaTimer::GetDeltaTime();

	switch (meleeAttackState_)
	{
	case MeleeAttackState::StartupTime:

		if (timer_ >= data.attackStartupTime) {
			boss_->ForcePlayByName("Jump_Land");
			ChangeAttackState(MeleeAttackState::Attack);
		}
		break;
	case MeleeAttackState::Attack:

		if (timer_ >= data.attackTime) {
			boss_->PlayByName("Idle");
			boss_->GetAttackCollider()->SetActive(true);
			boss_->GetAttackCollider()->SetColliderSize(data.attackColliderSize);
			boss_->GetAttackCollider()->SetColliderOffset(data.attackColliderOffset);
			ChangeAttackState(MeleeAttackState::LandingRecover);
		}
		break;
	case MeleeAttackState::LandingRecover:

		boss_->GetAttackCollider()->SetActive(false);
		if (timer_ >= data.attackRecoveryTime) {
			ChangeAttackState(MeleeAttackState::Finish);
		}
		break;
	case MeleeAttackState::Finish:

		boss_->ChangeState(std::make_unique<BossMoveState>(boss_));

		break;
	default:
		break;
	}

}

void BossMeleeState::Draw()
{
}

void BossMeleeState::ChangeAttackState(MeleeAttackState newState)
{
	meleeAttackState_ = newState;
	timer_ = 0.0f;
}
