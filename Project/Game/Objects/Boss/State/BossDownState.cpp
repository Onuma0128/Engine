#include "BossDownState.h"

#include "DeltaTimer.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Boss/State/BossMoveState.h"
#include "GameCamera/GameCamera.h"

BossDownState::BossDownState(BossEnemy* boss) : BossBaseState(boss) {}

void BossDownState::Init()
{
	boss_->PlayByName("No");
	boss_->GetEffect()->OnceDownEffect();

	// データを取得する
	const auto& data = boss_->GetItems()->GetDownData();
	boss_->GetGameCamera()->SetShake(data.shakePower);
}

void BossDownState::Finalize()
{
	boss_->GetEffect()->EmitDownStar(false);
	boss_->GetAudio()->StopAudio("BossDown.wav");
}

void BossDownState::Update()
{
	// データを取得する
	const auto& data = boss_->GetItems()->GetDownData();

	// タイムを加算
	timer_ += DeltaTimer::GetDeltaTime();

	// エフェクトを更新
	boss_->GetEffect()->EmitDownStar(true);

	// ダウンのSEがなっていなければ鳴らす
	const auto& volume = boss_->GetItems()->GetSeVolumeData();
	if (!boss_->GetAudio()->IsPlaying("BossDown.wav")) {
		boss_->GetAudio()->SoundPlayWave("BossDown.wav", volume.down);
	}

	// 時間が経ったらステートを戻す
	if (timer_ >= data.downTime) {
		boss_->GetEffect()->EmitDownStar(false);
		boss_->ChangeState(std::make_unique<BossMoveState>(boss_));
	}
}

void BossDownState::Draw()
{
}
