#include "BossDownState.h"

#include "DeltaTimer.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Boss/State/BossMoveState.h"

BossDownState::BossDownState(BossEnemy* boss) : BossBaseState(boss) {}

void BossDownState::Init()
{
	boss_->PlayByName("No");
}

void BossDownState::Finalize()
{
}

void BossDownState::Update()
{
	// データを取得する
	const auto& data = boss_->GetItems()->GetDownData();

	// タイムを加算
	timer_ += DeltaTimer::GetDeltaTime();

	// 時間が経ったらステートを戻す
	if (timer_ >= data.downTime) {
		boss_->ChangeState(std::make_unique<BossMoveState>(boss_));
	}
}

void BossDownState::Draw()
{
}
