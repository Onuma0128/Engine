#include "CompanionDashState.h"

#include "DeltaTimer.h"

#include "Objects/Player/Player.h"
#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "Objects/MuscleCompanion/State/CompanionMoveState.h"
#include "Objects/MuscleCompanion/State/CompanionPushUpIdleState.h"

CompanionDashState::CompanionDashState(MuscleCompanion* companion) : CompanionBaseState(companion) {}

void CompanionDashState::Init()
{
	// アニメーションを変更
	companion_->PlayByName("Run");
	companion_->GetMaterial().outlineColor = NumaEngine::Vector3::ExprZero;
	// 集合要求フラグをfalseにする
	companion_->SetGatherRequested(false);
	// 元の場所に戻ったフラグをfalseにする
	companion_->SetReturnOriginal(false);
	// ダッシュ用のコライダースケールを設定する
	companion_->SetColliderScale(
		companion_->GetItems()->GetDashData().dashColliderScale * companion_->GetTransform().scale_.x
	);

	// 向きをプレイヤーと同じにする
	const auto& player = companion_->GetPlayer();
	NumaEngine::Vector3 targetPosition = player->GetTransform().translation_;
	const float distance = companion_->GetItems()->GetDashData().dashTargetDistance;

	if (!player->GetShot()->GetTargetCollider()) {
		Matrix4x4 rotate = NumaEngine::Quaternion::MakeRotateMatrix(player->GetTransform().rotation_);
		targetPosition += (NumaEngine::Vector3::ExprUnitZ * distance).Transform(rotate);
	} else {
		targetCollider_ = player->GetShot()->GetTargetCollider();
		// 探索を更新する
		companion_->ResetSearch(targetCollider_->GetCenterPosition());
	}

	// 攻撃の速度ベクトルを設定する
	velocity_ = targetPosition - companion_->GetTransform().translation_;
	velocity_.y = 0.0f;
	velocity_ = velocity_.Normalize();
	// 攻撃時の回転を設定する
	yRotation_ = NumaEngine::Quaternion::DirectionToQuaternion(
		companion_->GetTransform().rotation_, velocity_, 1.0f);
	companion_->SetTransformRotation(yRotation_);
	companion_->SetDashDirection(velocity_);
}

void CompanionDashState::Finalize()
{
	// コライダースケールを元に戻す
	companion_->SetColliderScale(1.0f);
}

void CompanionDashState::Update()
{
	// スピードを取得
	const float speed = companion_->GetItems()->GetDashData().dashSpeed * companion_->GetTransform().scale_.x;

	if (targetCollider_) {
		// データを取得する
		const auto& data = companion_->GetItems()->GetMainData();
		// 時間を更新
		searchTimer_ += DeltaTimer::GetDeltaTime();
		// 更新時間が来たら探索を再更新する
		if (searchTimer_ > data.searchUpdateTime) {
			companion_->ResetSearch(targetCollider_->GetCenterPosition());
			searchTimer_ = 0.0f;
		}

		// 経路探索の更新
		companion_->GetPathFinder().Update(speed);
		companion_->GetPathFinder().DebugSpline(data.debugSpline);

		// 移動の更新
		velocity_ = companion_->GetPathFinder().GetVelocity();
		velocity_.y = 0.0f;
		if (velocity_.Length() != 0.0f) { velocity_ = velocity_.Normalize(); }

		// 移動時の回転の更新
		if (velocity_.Length() != 0.0f) {
			NumaEngine::Quaternion yRotation = companion_->GetPathFinder().GetRotation();
			companion_->SetTransformRotation(NumaEngine::Quaternion::Slerp(companion_->GetTransform().rotation_, yRotation, 0.2f));
		}
		if (!targetCollider_->GetActive()) {
			companion_->SetGatherRequested(true);
			companion_->ChangeState(std::make_unique<CompanionMoveState>(companion_));
			return;
		}
	}

	// 移動を更新
	NumaEngine::Vector3 position = companion_->GetTransform().translation_ + velocity_ * speed * DeltaTimer::GetDeltaTime();
	NumaEngine::Vector2 min = companion_->GetPlayer()->GetItem()->GetPlayerData().minPlayerClamp;
	NumaEngine::Vector2 max = companion_->GetPlayer()->GetItem()->GetPlayerData().maxPlayerClamp;
	position.x = std::clamp(position.x, min.x, max.x);
	position.z = std::clamp(position.z, min.y, max.y);
	if(position.x == min.x || position.x == max.x || position.z == min.y || position.z == max.y){
		companion_->ChangeState(std::make_unique<CompanionPushUpIdleState>(companion_));
		return;
	}
	companion_->SetTransformTranslation(position);
	// エフェクトを追加する
	companion_->GetEffect()->OnceMoveEffect();
	companion_->GetEffect()->OnceDashEffect(false);
}

void CompanionDashState::Draw()
{
}

