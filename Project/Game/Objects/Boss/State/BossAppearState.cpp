#include "BossAppearState.h"

#include <numbers>

#include "DeltaTimer.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Boss/State/BossMoveState.h"
#include "Objects/Player/Player.h"
#include "GameCamera/GameCamera.h"

BossAppearState::BossAppearState(BossEnemy* boss) : BossBaseState(boss) {}

void BossAppearState::Init()
{
	// データを取得する
	const auto& mainData = boss_->GetItems()->GetMainData();
	const auto& appearData = boss_->GetItems()->GetAppearData();

	// プレイヤーとの距離を測る
	float dist = Vector3::Distance(mainData.startPosition, boss_->GetPlayer()->GetTransform().translation_);
	float distSab = Vector3::Distance(mainData.sabStartPosition, boss_->GetPlayer()->GetTransform().translation_);
	
	// プレイヤーが遠い方にする
	if (dist > distSab) {
		boss_->SetTransformTranslation(mainData.startPosition);
	} else {
		boss_->SetTransformTranslation(mainData.sabStartPosition);
	}
	boss_->SetTransformRotation(Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, std::numbers::pi_v<float>));

	// ジャンプ関連のパラメーター取得
	jumpVelocityY_ = appearData.jumpVelocityY;
	jumpAccelerY_ = appearData.jumpAccelerY;

	// 開始Y座標を初期化
	startY_ = 0.0f;
}

void BossAppearState::Finalize()
{
}

void BossAppearState::Update()
{
	// データを取得する
	const auto& data = boss_->GetItems()->GetAppearData();
	const auto& volume = boss_->GetItems()->GetSeVolumeData();

	// タイムを加算
	timer_ += DeltaTimer::GetDeltaTime();

	switch (appearState_)
	{
	case AppearState::StartupTime:
		if (timer_ >= data.startupTime) {
			ChangeAppearState(AppearState::fallDownTime);
		}
		break;
	case AppearState::fallDownTime:

		// 落下処理
		UpdateJump();

		if (timer_ >= data.fallDownTime && boss_->GetTransform().translation_.y <= startY_) {
			Vector3 translate = boss_->GetTransform().translation_;
			translate.y = startY_;
			boss_->SetTransformTranslation(translate);
			boss_->GetEffect()->OnceJumpEffect();
			boss_->GetGameCamera()->SetShake(data.shakePower);
			boss_->GetAudio()->SoundPlayWave("BossLanding.wav", volume.landing);
			ChangeAppearState(AppearState::Recover);
		}
		break;
	case AppearState::Recover:

		if (timer_ >= data.recoveryTime) {
			ChangeAppearState(AppearState::Finish);
		}
		break;
	case AppearState::Finish:

		// 登場が終わったらステートを変化させる
		boss_->ChangeState(std::make_unique<BossMoveState>(boss_));

		break;
	default:
		break;
	}

}

void BossAppearState::Draw()
{
}

void BossAppearState::ChangeAppearState(AppearState newState)
{
	appearState_ = newState;
	timer_ = 0.0f;
}

void BossAppearState::UpdateJump()
{
	// 速度更新（加速度）
	jumpVelocityY_ += jumpAccelerY_ * DeltaTimer::GetDeltaTime();

	// 位置更新（Y方向）
	Vector3 translate = boss_->GetTransform().translation_;
	translate.y += jumpVelocityY_ * DeltaTimer::GetDeltaTime();
	boss_->SetTransformTranslation(translate);
}