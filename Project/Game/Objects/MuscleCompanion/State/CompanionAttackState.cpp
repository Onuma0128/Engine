#include "CompanionAttackState.h"

#include "DeltaTimer.h"

#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "Objects/MuscleCompanion/State/CompanionIdleState.h"
#include "Objects/MuscleCompanion/Collider/CompanionAttackCollider.h"

CompanionAttackState::CompanionAttackState(MuscleCompanion* companion) : CompanionBaseState(companion) {}

void CompanionAttackState::Init()
{
	companion_->PlayByName("Idle");
	ChangeAttackState(AttackState::StartUp);

	const auto& colliders = companion_->GetAttackCollider()->GetHitColliders();
	for (const auto& collider : colliders) {
		if (collider->GetActive()) {
			Vector3 velocity = collider->GetCenterPosition() - companion_->GetTransform().translation_;
			Quaternion yRotation_ = Quaternion::DirectionToQuaternion(
				companion_->GetTransform().rotation_, velocity.Normalize(), 1.0f);
			companion_->SetTransformRotation(yRotation_);
			break;
		}
	}
}

void CompanionAttackState::Finalize()
{
	companion_->SetFirstDashAttack(false);
	companion_->GetAttackCollider()->SetActive(false);
}

void CompanionAttackState::Update()
{
	// データを取得する
	const auto& data = companion_->GetItems()->GetAttackData();

	// タイムを加算
	timer_ += DeltaTimer::GetDeltaTime();

	// ダッシュ攻撃の一回目なら硬直時間を与える
	if (companion_->GetFirstDashAttack()) {
		if (timer_ > data.firstDashTime) {
			companion_->SetFirstDashAttack(false);
		} else {
			return;
		}
	}

	// 攻撃ステートごとの処理
	switch (attackState_)
	{
	case AttackState::StartUp:

		if (timer_ >= data.attackStartupTime) {
			companion_->GetAttackCollider()->SetActive(true);
			companion_->PlayByName("Punch");
			ChangeAttackState(AttackState::Active);
		}
		break;
	case AttackState::Active:

		if (timer_ >= data.attackActiveTime) {
			companion_->GetAttackCollider()->SetActive(false);
			companion_->PlayByName("Idle");
			ChangeAttackState(AttackState::Recovery);
		}
		break;
	case AttackState::Recovery:

		if (timer_ >= data.attackRecoveryTime) {
			companion_->PlayByName("Idle");
			ChangeAttackState(AttackState::Finish);
		}
		break;
	case AttackState::Finish:
	{
		// nullポインタチェック
		if (companion_->GetAttackCollider()->GetHitColliders().empty()) {
			companion_->GetAttackCollider()->ResetHitColliders();
			companion_->ChangeState(std::make_unique<CompanionIdleState>(companion_));
			return;
		}

		// 攻撃が当たっていたら攻撃状態へ、当たっていなければ待機状態へ
		const auto& colliders = companion_->GetAttackCollider()->GetHitColliders();
		for (const auto& collider : colliders) {
			if (collider->GetActive()) {
				companion_->ChangeState(std::make_unique<CompanionAttackState>(companion_));
				return;
			}
		}
		companion_->GetAttackCollider()->ResetHitColliders();
		companion_->ChangeState(std::make_unique<CompanionIdleState>(companion_));
		return;

	}
		break;
	default:
		break;
	}
}

void CompanionAttackState::Draw()
{
}

void CompanionAttackState::ChangeAttackState(AttackState newState)
{
	attackState_ = newState;
	timer_ = 0.0f;
}


