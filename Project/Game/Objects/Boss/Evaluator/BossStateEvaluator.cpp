#include "BossStateEvaluator.h"

#include "imgui.h"

#include "DeltaTimer.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Boss/State/BossJumpAttackState.h"
#include "Objects/Boss/State/BossDashAttackState.h"
#include "Objects/Boss/State/BossEnemySpawnState.h"

#include "Objects/Player/Player.h"

void BossStateEvaluator::Initialize()
{
	jumpAttackScore_ = 0;
	dashAttackScore_ = 0;
	spawnScore_ = 0;
}

void BossStateEvaluator::Update()
{
	// データを取得する
	const auto& data = boss_->GetItems()->GetStateScoreData();

	// タイム加算
	timer_ += DeltaTimer::GetDeltaTime();

	// タイムが超えたら
	if (timer_ > data.scoreAccumulationTime) {
		// プレイヤーとの距離を計算
		float distance = Vector3::Distance(
			boss_->GetTransform().translation_, boss_->GetPlayer()->GetTransform().translation_);
		// プレイヤーとの距離でスコアを加算する
		DistanceScore(data, BossState::EnemySpawn, distance);
		DistanceScore(data, BossState::JumpAttack, distance);
		DistanceScore(data, BossState::DashAttack, distance);
		// プレイヤーを見たかでスコアを加算する
		LookingScore(data);
		// ボスのHPに応じてスコアを加算する
		BossHpScore(data);
		// リセット
		timer_ = 0.0f;

	} else {
		// 見ていたらそのターンフラグを立てる
		if (!isLook_) {
			isLook_ = boss_->GetLooking();
		}
	}

	// ステートを変化させる
	ScoreChangeToState();
}

void BossStateEvaluator::DrawImGui() 
{
	ImGui::Begin("BossScore");
	ImGui::Text("SpawnScore : %u", static_cast<int>(spawnScore_));
	ImGui::Text("\n");
	ImGui::Text("DashAttackScore : %u", static_cast<int>(dashAttackScore_));
	ImGui::Text("\n");
	ImGui::Text("JumpAttackScore : %u", static_cast<int>(jumpAttackScore_));
	ImGui::End();
}

void BossStateEvaluator::BossHpScore(const BossStateScoreData& data)
{
	const auto& mainData = boss_->GetItems()->GetMainData();
	uint32_t maxHp = mainData.maxHP;
	uint32_t currentHp = boss_->GetCurrentHp();

	if ((maxHp / 2) < currentHp) {
		jumpAttackScore_ += data.jumpAttackHpHigh;
		dashAttackScore_ += data.dashAttackHpHigh;
		spawnScore_ += data.spawnHpLow;
	} else {
		jumpAttackScore_ += data.jumpAttackHpLow;
		dashAttackScore_ += data.dashAttackHpLow;
		spawnScore_ += data.spawnHpHigh;
	}
}

void BossStateEvaluator::DistanceScore(const BossStateScoreData& data, BossState state, float distance)
{
	// ステートによってスコアを加算する
	switch (state)
	{
	case BossState::EnemySpawn:

		if (distance < data.playerToDistance) {
			spawnScore_ += static_cast<uint32_t>(data.spawnDistanceLow);
		} else {
			spawnScore_ += static_cast<uint32_t>(data.spawnDistanceHigh);
		}

		break;
	case BossState::DashAttack:

		if (distance < data.playerToDistance) {
			dashAttackScore_ += static_cast<uint32_t>(data.dashAttackDistanceHigh);
		} else {
			dashAttackScore_ += static_cast<uint32_t>(data.dashAttackDistanceLow);
		}

		break;
	case BossState::JumpAttack:

		if (distance < data.playerToDistance) {
			jumpAttackScore_ += static_cast<uint32_t>(data.jumpAttackDistanceLow);
		} else {
			jumpAttackScore_ += static_cast<uint32_t>(data.jumpAttackDistanceHigh);
		}

		break;
	default:
		break;
	}
}

void BossStateEvaluator::LookingScore(const BossStateScoreData& data)
{
	if (isLook_) {
		jumpAttackScore_ += data.jumpAttackLookLow;
		dashAttackScore_ += data.dashAttackLookHigh;
		spawnScore_ += data.spawnLookLow;
	} else {
		jumpAttackScore_ += data.jumpAttackLookHigh;
		dashAttackScore_ += data.dashAttackLookLow;
		spawnScore_ += data.spawnLookHigh;
	}
	isLook_ = false;
}

void BossStateEvaluator::ScoreChangeToState()
{
	if (spawnScore_ >= 100) {
		spawnScore_ = 0;
		newBossStates_.push_back(BossState::EnemySpawn);
	} 
	if (jumpAttackScore_ >= 100) {
		jumpAttackScore_ = 0;
		newBossStates_.push_back(BossState::JumpAttack);
	} 
	if (dashAttackScore_ >= 100) {
		dashAttackScore_ = 0;
		newBossStates_.push_back(BossState::DashAttack);
	}

	if (!newBossStates_.empty()) {
		auto state = newBossStates_.front();
		switch (state)
		{
		case BossState::EnemySpawn:
			boss_->ChangeState(std::make_unique<BossEnemySpawnState>(boss_));
			break;
		case BossState::DashAttack:
			boss_->ChangeState(std::make_unique<BossDashAttackState>(boss_));
			break;
		case BossState::JumpAttack:
			boss_->ChangeState(std::make_unique<BossJumpAttackState>(boss_));
			break;
		default:
			break;
		}
		newBossStates_.pop_front();
	}
}
