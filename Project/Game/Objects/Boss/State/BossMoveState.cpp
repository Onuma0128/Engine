#include "BossMoveState.h"

#include "DeltaTimer.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Player/Player.h"
#include "Objects/Boss/State/BossEnemySpawnState.h"
#include "Objects/Boss/Evaluator/BossStateEvaluator.h"

BossMoveState::BossMoveState(BossEnemy* boss) : BossBaseState(boss) {}

void BossMoveState::Init()
{
	// アニメーションを変更
	boss_->PlayByName("Walk");
	boss_->GetMaterial().outlineColor = Vector3::ExprUnitZ;
	// 探索を更新する
	boss_->ResetSearch(boss_->GetPlayer()->GetTransform().translation_);
}

void BossMoveState::Finalize()
{
}

void BossMoveState::Update()
{
	// データを取得する
	const auto& data = boss_->GetItems()->GetMainData();
	// 時間を更新
	searchTimer_ += DeltaTimer::GetDeltaTime();
	spawnTimer_ += DeltaTimer::GetDeltaTime();
	// 更新時間が来たら探索を再更新する
	if (searchTimer_ > data.searchUpdateTime) {
		boss_->ResetSearch(boss_->GetPlayer()->GetTransform().translation_);
		searchTimer_ = 0.0f;
	}

	// スピードを取得
	float speed = data.speed;
	// 経路探索の更新
	boss_->GetPathFinder().Update(speed);
	boss_->GetPathFinder().DebugSpline(data.debugSpline);

	// 移動の更新
	Vector3 velocity = boss_->GetPathFinder().GetVelocity();
	velocity.y = 0.0f;
	if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }
	Vector3 translate = boss_->GetTransform().translation_ +
		speed * velocity * DeltaTimer::GetDeltaTime();
	boss_->SetTransformTranslation(translate);

	// 移動時の回転の更新
	if (velocity.Length() != 0.0f) {
		Quaternion yRotation = boss_->GetPathFinder().GetRotation();
		boss_->SetTransformRotation(Quaternion::Slerp(boss_->GetTransform().rotation_, yRotation, 0.2f));
	}

	// ステートを変更する
	boss_->GetStateEvaluator()->Update();
}

void BossMoveState::Draw()
{
}
