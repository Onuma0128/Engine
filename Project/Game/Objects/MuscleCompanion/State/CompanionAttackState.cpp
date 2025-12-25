#include "CompanionAttackState.h"

#include "DeltaTimer.h"

#include "Objects/Player/Player.h"
#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"

CompanionAttackState::CompanionAttackState(MuscleCompanion* companion) : CompanionBaseState(companion) {}

void CompanionAttackState::Init()
{
	// アニメーションを変更
	companion_->PlayByName("Run_Arms");

	// 集合要求フラグをfalseにする
	companion_->SetGatherRequested(false);

	// 向きをプレイヤーと同じにする
	const auto& player = companion_->GetPlayer();
	Vector3 targetPosition = player->GetTransform().translation_;

	if (player->GetShot()->GetTargetPosition().Length() == 0.0f) {
		Matrix4x4 rotate = Quaternion::MakeRotateMatrix(player->GetTransform().rotation_);
		targetPosition += (Vector3::ExprUnitZ * 15.0f).Transform(rotate);
	} else {
		targetPosition = player->GetShot()->GetTargetPosition();
	}

	// 攻撃の速度ベクトルを設定する
	velocity_ = targetPosition - companion_->GetTransform().translation_;
	velocity_.y = 0.0f;
	velocity_ = velocity_.Normalize();
	// 攻撃時の回転を設定する
	yRotation_ = Quaternion::DirectionToQuaternion(
		companion_->GetTransform().rotation_, velocity_, 1.0f);
}

void CompanionAttackState::Finalize()
{
}

void CompanionAttackState::Update()
{
	// 回転を更新
	companion_->SetTransformRotation(
		Quaternion::Slerp(companion_->GetTransform().rotation_, yRotation_, 0.2f));

	// 移動を更新
	companion_->SetTransformTranslation(
		companion_->GetTransform().translation_ + velocity_ *
		companion_->GetItems()->GetMainData().dashSpeed * DeltaTimer::GetDeltaTime());
}

void CompanionAttackState::Draw()
{
}


