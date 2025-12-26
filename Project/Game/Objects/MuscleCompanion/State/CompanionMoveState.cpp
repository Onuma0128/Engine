#include "CompanionMoveState.h"

#include <memory>

#include "DeltaTimer.h"

#include "Objects/Player/Player.h"
#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "Objects/MuscleCompanion/State/CompanionIdleState.h"
#include "Objects/MuscleCompanion/State/CompanionDashState.h"
#include "Objects/MuscleCompanion/State/CompanionAttackState.h"	

CompanionMoveState::CompanionMoveState(MuscleCompanion* companion) : CompanionBaseState(companion) {}

void CompanionMoveState::Init()
{
	// アニメーションを変更
	companion_->PlayByName("Run");
	companion_->GetMaterial().outlineColor = Vector3::ExprUnitZ;
	// 探索を更新する
	companion_->ResetSearch();
}

void CompanionMoveState::Finalize()
{
}

void CompanionMoveState::Update()
{
	// データを取得する
	const auto& data = companion_->GetItems()->GetMainData();
	// 時間を更新
	searchTimer_ += DeltaTimer::GetDeltaTime();
	// 更新時間が来たら探索を再更新する
	if (searchTimer_ > data.searchUpdateTime) {
		companion_->ResetSearch();
		searchTimer_ = 0.0f;
	}
	// 経路探索の更新
	companion_->GetPathFinder().Update(data.speed);
	companion_->GetPathFinder().DebugSpline(data.debugSpline);

	// 移動の更新
	Vector3 velocity = companion_->GetPathFinder().GetVelocity();
	velocity.y = 0.0f;
	if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }
	Vector3 translate = companion_->GetTransform().translation_ +
		data.speed * velocity * DeltaTimer::GetDeltaTime();
	companion_->SetTransformTranslation(translate);

	// 移動時の回転の更新
	if (velocity.Length() != 0.0f) {
		Quaternion yRotation = companion_->GetPathFinder().GetRotation();
		companion_->SetTransformRotation(Quaternion::Slerp(companion_->GetTransform().rotation_, yRotation, 0.2f));
	}

	// 距離が近づいたら待機ステートに遷移する
	if (!companion_->SearchDistance()) {
		companion_->ChangeState(std::make_unique<CompanionIdleState>(companion_));
		return;
	}

	// プレイヤーが指示を出したら攻撃ステートに遷移する
	if (companion_->GetPlayer()->GetShot()->GetIsShot()) {
		companion_->ChangeState(std::make_unique<CompanionDashState>(companion_));
		companion_->GetPlayer()->GetShot()->SetIsShot(false);
		return;
	}
}

void CompanionMoveState::Draw()
{
}
