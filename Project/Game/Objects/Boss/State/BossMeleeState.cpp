#include "BossMeleeState.h"

#include "DeltaTimer.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Boss/State/BossMoveState.h"
#include "GameCamera/GameCamera.h"

BossMeleeState::BossMeleeState(BossEnemy* boss) : BossBaseState(boss) {}

void BossMeleeState::Init()
{
	// アニメーションの初期化
	boss_->ForcePlayByName("Jump");
	boss_->GetTimeStop() = true;
	boss_->GetEffect()->SetAttackEffect(BossAttackEffect::Melee);

	// ジャンプ関連のパラメーター取得
	const auto& data = boss_->GetItems()->GetMeleeData();
	jumpVelocityY_ = data.jumpVelocityY;
	jumpAccelerY_ = data.jumpAccelerY;

	// 開始Y座標を初期化
	startY_ = boss_->GetTransform().translation_.y;
}

void BossMeleeState::Finalize()
{
	boss_->GetAttackCollider()->SetActive(false);
	boss_->GetEffect()->AttackEffectReset();
}

void BossMeleeState::Update()
{
	// データを取得する
	const auto& data = boss_->GetItems()->GetMeleeData();
	const auto& volume = boss_->GetItems()->GetSeVolumeData();

	// タイムを加算
	timer_ += DeltaTimer::GetDeltaTime();

	switch (meleeAttackState_)
	{
	case MeleeAttackState::StartupTime:

		UpdateJump();

		boss_->GetEffect()->SetAttackEffect(BossAttackEffect::Melee);

		if (timer_ >= data.attackStartupTime) {
			boss_->ForcePlayByName("Jump_Land");
			boss_->GetTimeStop() = false;
			boss_->SetAnimationTime(0.0f);
			ChangeAttackState(MeleeAttackState::Attack);
		}
		break;
	case MeleeAttackState::Attack:

		UpdateJump();

		if (timer_ >= data.attackTime && boss_->GetTransform().translation_.y <= startY_) {
			boss_->PlayByName("Idle");
			boss_->GetTimeStop() = false;
			boss_->SetAnimationTime(1.0f);
			boss_->GetEffect()->AttackEffectReset();
			boss_->GetEffect()->OnceJumpEffect();
			boss_->GetGameCamera()->SetShake(data.shakePower);
			boss_->GetAudio()->SoundPlayWave("BossLanding.wav", volume.landing);
			Vector3 translate = boss_->GetTransform().translation_;
			translate.y = startY_;
			boss_->SetTransformTranslation(translate);
			boss_->GetAttackCollider()->SetActive(true);
			boss_->GetAttackCollider()->SetColliderSize(data.attackColliderSize);
			boss_->GetAttackCollider()->SetColliderOffset(data.attackColliderOffset);
			ChangeAttackState(MeleeAttackState::LandingRecover);
		}
		break;
	case MeleeAttackState::LandingRecover:

		boss_->GetAttackCollider()->SetActive(false);
		// 近接攻撃の終わりにスコア計算をしてステートを変化させる
		if (timer_ >= data.attackRecoveryTime) {
			boss_->GetStateEvaluator()->UpdateScore();
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

void BossMeleeState::UpdateJump()
{
	// 速度更新（加速度）
	jumpVelocityY_ += jumpAccelerY_ * DeltaTimer::GetDeltaTime();

	// 位置更新（Y方向）
	Vector3 translate = boss_->GetTransform().translation_;
	translate.y += jumpVelocityY_ * DeltaTimer::GetDeltaTime();
	boss_->SetTransformTranslation(translate);
}

void BossMeleeState::ChangeAttackState(MeleeAttackState newState)
{
	meleeAttackState_ = newState;
	timer_ = 0.0f;
}
