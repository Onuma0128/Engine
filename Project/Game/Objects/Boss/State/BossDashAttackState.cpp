#include "BossDashAttackState.h"

#include <memory>

#include "DeltaTimer.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Boss/State/BossMoveState.h"
#include "Objects/Player/Player.h"

BossDashAttackState::BossDashAttackState(BossEnemy* boss) : BossBaseState(boss) {}

void BossDashAttackState::Init()
{
	if (!boss_->GetLooking()) {
		boss_->ChangeState(std::make_unique<BossMoveState>(boss_));
		return;
	}

	boss_->PlayByName("Run");
}

void BossDashAttackState::Finalize()
{
	boss_->GetAttackCollider()->SetActive(false);
}

void BossDashAttackState::Update()
{
	// データを取得する
	const auto& data = boss_->GetItems()->GetDashAttackData();

	// タイムを加算
	timer_ += DeltaTimer::GetDeltaTime();

	switch (dashAttackState_)
	{
	case DashAttackState::StartupTime:
	{
		// プレイヤーの方向に回転をする
		Vector3 translate = boss_->GetTransform().translation_;
		Vector3 target = boss_->GetPlayer()->GetTransform().translation_;
		target.y = translate.y;
		float distance = Vector3::Distance(target, translate);
		Vector3 velocity = (target - translate);
		velocity.y = 0.0f;

		// 回転の更新
		if (distance > 0.1f) {
			Quaternion yRotation = Quaternion::DirectionToQuaternion(Quaternion::IdentityQuaternion(), velocity, 1.0f);
			boss_->SetTransformRotation(Quaternion::Slerp(boss_->GetTransform().rotation_, yRotation, 0.2f));
		}

		if (timer_ >= data.attackStartupTime) {
			CreateVelocity();
			boss_->PlayByName("Run_Attack");
			boss_->GetAttackCollider()->SetActive(true);
			ChangeAttackState(DashAttackState::DashAttack);
		}
	}
		break;
	case DashAttackState::DashAttack:
	{
		// 移動の更新
		Vector3 translate = boss_->GetTransform().translation_;
		translate += velocity_ * data.dashSpeed * DeltaTimer::GetDeltaTime();
		boss_->SetTransformTranslation(translate);
		boss_->GetAttackCollider()->SetColliderSize(data.attackColliderSize);
		boss_->GetAttackCollider()->SetColliderOffset(data.attackColliderOffset);

		if (timer_ >= data.dashTime) {
			boss_->PlayByName("Idle");
			boss_->GetAttackCollider()->SetActive(false);
			ChangeAttackState(DashAttackState::LandingRecover);
		}
	}
		break;
	case DashAttackState::LandingRecover:

		if (timer_ >= data.attackRecoveryTime) {
			ChangeAttackState(DashAttackState::Finish);
		}
		break;
	case DashAttackState::Finish:

		// ステートを変える
		boss_->ChangeState(std::make_unique<BossMoveState>(boss_));

		break;
	default:
		break;
	}
}

void BossDashAttackState::Draw()
{
}

void BossDashAttackState::ChangeAttackState(DashAttackState newState)
{
	dashAttackState_ = newState;
	timer_ = 0.0f;
}

void BossDashAttackState::CreateVelocity() 
{
	Vector3 translate = boss_->GetTransform().translation_;
	Vector3 target = boss_->GetPlayer()->GetTransform().translation_;
	velocity_ = (target - translate);
	velocity_.y = 0.0f;
	if (velocity_.Length() != 0.0f) { velocity_ = velocity_.Normalize(); }
	else { velocity_ = -Vector3::ExprUnitZ; }
}