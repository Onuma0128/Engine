#include "BossJumpAttackState.h"

#include "DeltaTimer.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Boss/State/BossMoveState.h"
#include "Objects/Player/Player.h"
#include "GameCamera/GameCamera.h"

BossJumpAttackState::BossJumpAttackState(BossEnemy* boss) : BossBaseState(boss) {}

void BossJumpAttackState::Init()
{
	// アニメーションの初期化
	boss_->PlayByName("Wave");
	boss_->GetEffect()->SetAttackEffect(BossAttackEffect::JumpAttack);

	// ジャンプ関連のパラメーター取得
	const auto& data = boss_->GetItems()->GetJumpAttackData();
	jumpVelocityY_ = data.jumpVelocityY;
	jumpAccelerY_ = data.jumpAccelerY;

	// 開始Y座標を初期化
	startY_ = boss_->GetTransform().translation_.y;
}

void BossJumpAttackState::Finalize()
{
	boss_->GetAttackCollider()->SetActive(false);
	boss_->GetEffect()->AttackEffectReset();
}

void BossJumpAttackState::Update()
{
	// データを取得する
	const auto& data = boss_->GetItems()->GetJumpAttackData();
	const auto& volume = boss_->GetItems()->GetSeVolumeData();

	// タイムを加算
	timer_ += DeltaTimer::GetDeltaTime();


	switch (jumpAttackState_)
	{
	case JumpAttackState::StartupTime:

		if (timer_ >= data.attackStartupTime) {
			boss_->ForcePlayByName("Jump");
			boss_->GetTimeStop() = true;
			ChangeAttackState(JumpAttackState::JumpUp);
		}
		break;
	case JumpAttackState::JumpUp:

		// ジャンプ処理の更新
		UpdateJump();

		if (timer_ >= data.JumpUpTime) {
			boss_->GetTimeStop() = false;
			boss_->PlayByName("Run");
			// 効果音を鳴らす
			boss_->GetAudio()->SoundPlayWave("BossAttackCaveat.wav", volume.attackCaveat);
			ChangeAttackState(JumpAttackState::AirHold);
		}
		break;
	case JumpAttackState::AirHold:
	{
		// プレイヤーに追従をする
		Vector3 translate = boss_->GetTransform().translation_;
		Vector3 target = boss_->GetPlayer()->GetTransform().translation_;
		target.y = translate.y;
		float distance = Vector3::Distance(target, translate);
		Vector3 velocity = (target - translate);
		velocity.y = 0.0f;

		// エフェクトの更新
		boss_->GetEffect()->SetAttackEffect(BossAttackEffect::JumpAttack);

		// 移動と回転の更新
		if (distance > 0.1f) {
			translate += velocity.Normalize() * data.airSpeed * DeltaTimer::GetDeltaTime();
			boss_->SetTransformTranslation(translate);
			Quaternion yRotation = Quaternion::DirectionToQuaternion(Quaternion::IdentityQuaternion(),velocity, 1.0f);
			boss_->SetTransformRotation(Quaternion::Slerp(boss_->GetTransform().rotation_, yRotation, 0.2f));
		}

		if (timer_ >= data.airHoldTime) {
			boss_->PlayByName("Idle");
			ChangeAttackState(JumpAttackState::FallDown);
		}
	}
		break;
	case JumpAttackState::FallDown:

		// ジャンプ処理の更新
		UpdateJump();
		// エフェクトの更新
		boss_->GetEffect()->SetAttackEffect(BossAttackEffect::JumpAttack);

		if (timer_ >= data.fallDownTime && boss_->GetTransform().translation_.y <= startY_) {
			Vector3 translate = boss_->GetTransform().translation_;
			translate.y = startY_;
			boss_->GetEffect()->AttackEffectReset();
			boss_->GetEffect()->OnceJumpEffect();
			boss_->GetGameCamera()->SetShake(data.shakePower);
			boss_->GetAudio()->SoundPlayWave("BossLanding.wav", volume.landing);
			boss_->GetAttackCollider()->SetActive(true);
			boss_->GetAttackCollider()->SetColliderSize(data.attackColliderSize);
			boss_->GetAttackCollider()->SetColliderOffset(data.attackColliderOffset);
			boss_->SetTransformTranslation(translate);
			ChangeAttackState(JumpAttackState::LandingRecover);
		}
		break;
	case JumpAttackState::LandingRecover:

		boss_->GetAttackCollider()->SetActive(false);
		if (timer_ >= data.attackRecoveryTime) {
			ChangeAttackState(JumpAttackState::Finish);
		}
		break;
	case JumpAttackState::Finish:

		// ステートを変える

		boss_->ChangeState(std::make_unique<BossMoveState>(boss_));
		break;
	default:
		break;
	}
}

void BossJumpAttackState::Draw()
{
}

void BossJumpAttackState::UpdateJump()
{
	// 速度更新（加速度）
	jumpVelocityY_ += jumpAccelerY_ * DeltaTimer::GetDeltaTime();

	// 位置更新（Y方向）
	Vector3 translate = boss_->GetTransform().translation_;
	translate.y += jumpVelocityY_ * DeltaTimer::GetDeltaTime();
	boss_->SetTransformTranslation(translate);
}

void BossJumpAttackState::ChangeAttackState(JumpAttackState newState)
{
	jumpAttackState_ = newState;
	timer_ = 0.0f;
}