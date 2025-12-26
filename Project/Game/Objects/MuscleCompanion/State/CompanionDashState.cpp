#include "CompanionDashState.h"

#include "DeltaTimer.h"

#include "Objects/Player/Player.h"
#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"

CompanionDashState::CompanionDashState(MuscleCompanion* companion) : CompanionBaseState(companion) {}

void CompanionDashState::Init()
{
	// アニメーションを変更
	companion_->PlayByName("Run_Arms");
	companion_->GetMaterial().outlineColor = Vector3::ExprUnitX;
	// 集合要求フラグをfalseにする
	companion_->SetGatherRequested(false);
	// ダッシュ用のコライダースケールを設定する
	companion_->SetDashColliderScale(companion_->GetItems()->GetDashData().dashColliderScale);

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
	companion_->SetTransformRotation(yRotation_);
}

void CompanionDashState::Finalize()
{
	// ダッシュ用のコライダースケールを元に戻す
	companion_->SetDashColliderScale(1.0f);
}

void CompanionDashState::Update()
{
	// 移動を更新
	companion_->SetTransformTranslation(
		companion_->GetTransform().translation_ + velocity_ *
		companion_->GetItems()->GetDashData().dashSpeed * DeltaTimer::GetDeltaTime());
}

void CompanionDashState::Draw()
{
}
