#include "GameSceneUIs.h"

#include "DeltaTimer.h"

#include "Objects/Player/Player.h"
#include "Objects/Enemy/Spawner/EnemySpawnerFactory.h"

#include "Uis/GameSceneUIs/PlayerControlUI.h"
#include "Uis/GameSceneUIs/PlayerMenuUI.h"

void GameSceneUIs::Init()
{
	// 調整項目の初期化
	items_ = std::make_unique<GameUiAdjustItem>();
	items_->LoadItems();

	// 操作UIの初期化
	controlUI_ = std::make_unique<PlayerControlUI>();
	controlUI_->Init();
	isDrawGameUIs_ = true;

	// シーンフェードを初期化
	sceneFade_ = std::make_unique<BaseUI>();
	sceneFade_->Init("GameFade", "GameData", true);
	sceneFade_->GetSprite()->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
	sceneFade_->FadeOut();

	bossFade_ = std::make_unique<BaseUI>();
	bossFade_->Init("BossFade", "GameData");

	selectSystem_ = std::make_unique<SelectSystem>();
	selectSystem_->Init();
	selectSystem_->SetSceneFade(sceneFade_.get());

	// Kill数UIの初期化
	killCountUI_ = std::make_unique<NumberCountUI>();
	killCountUI_->Init();
	maxKillCountUI_ = std::make_unique<NumberCountUI>();
	maxKillCountUI_->Init();
	catUI_ = std::make_unique<BaseUI>();
	catUI_->Init("CatUI", "GameData");

	Update();
}

void GameSceneUIs::Update()
{
#ifdef ENABLE_EDITOR
	items_->Editor();
#endif // ENABLE_EDITOR

	BossFadeUpdate();

	controlUI_->Update();

	selectSystem_->Update();
	
	sceneFade_->DrawImGui();
	sceneFade_->Update();
	bossFade_->DrawImGui();
	bossFade_->Update();

	// 調整項目をセットし、更新する
	killCountUI_->SetInterval(items_->GetSelectData().killNumberUiInterval);
	killCountUI_->SetSize(items_->GetSelectData().killNumberUiSize);
	killCountUI_->SetPosition(items_->GetSelectData().killNumberUiPos);
	killCountUI_->MochiPuniScale(knockdownCount_, items_->GetSelectData().killNumberUiSize.y);
	killCountUI_->Update(knockdownCount_);

	maxKillCountUI_->SetInterval(items_->GetSelectData().maxKillNumberUiInterval);
	maxKillCountUI_->SetSize(items_->GetSelectData().maxKillNumberUiSize);
	maxKillCountUI_->SetPosition(items_->GetSelectData().maxKillNumberUiPos);
	maxKillCountUI_->MochiPuniScale(items_->GetSelectData().maxKillCount, items_->GetSelectData().maxKillNumberUiSize.y);
	maxKillCountUI_->Update(items_->GetSelectData().maxKillCount);

	catUI_->Update();
}

void GameSceneUIs::BossFadeUpdate()
{
	// データを取得
	const auto& data = items_->GetSelectData();

	// タイムを加算
	if (bossFadeTime_ >= 0.0f) {
		bossFadeTime_ += DeltaTimer::GetDeltaTime();
		// キル数が一定以上になったら
	} else {
		uint32_t clearKill = static_cast<uint32_t>(player_->GetItem()->GetPlayerData().clearKill);
		if (spawner_->GetKnockdownCount() >= clearKill) {
			bossFadeTime_ = 0.0f;
			bossFade_->FadeIn();
		}
	}

	switch (bossFadeState_)
	{
	case GameSceneUIs::BossFadeState::StartIn:
		
		// 時間が来たらステートを変える
		if (bossFadeTime_ > data.startInTime) {
			isDrawGameUIs_ = false;
			bossFade_->FadeOut();
			ChangeFadeState(BossFadeState::StartOut);
		}
		break;
	case GameSceneUIs::BossFadeState::StartOut:

		// 時間が来たらステートを変える
		if (bossFadeTime_ > data.startOutTime) {
			ChangeFadeState(BossFadeState::Active);
		}
		break;
	case GameSceneUIs::BossFadeState::Active:

		// 時間が来たらステートを変える
		if (bossFadeTime_ > data.activeTime) {
			ChangeFadeState(BossFadeState::RecoverIn);
		}
		break;
	case GameSceneUIs::BossFadeState::RecoverIn:

		// 時間が来たらステートを変える
		if (bossFadeTime_ > data.recoverInTime) {
			bossFade_->FadeIn();
			ChangeFadeState(BossFadeState::RecoverOut);
		}
		break;
	case GameSceneUIs::BossFadeState::RecoverOut:

		// 時間が来たらステートを変える
		if (bossFadeTime_ > data.recoverOutTime) {
			bossFade_->FadeOut();
			isDrawGameUIs_ = true;
			ChangeFadeState(BossFadeState::End);
		}
		break;
	case GameSceneUIs::BossFadeState::End:
		break;
	default:
		break;
	}

}

void GameSceneUIs::Draw()
{
	if (isDrawGameUIs_) {
		controlUI_->Draw();

		killCountUI_->Draw();
		maxKillCountUI_->Draw();
		catUI_->Draw();
	}

	selectSystem_->Draw();

	bossFade_->Draw();
	sceneFade_->Draw();
}

void GameSceneUIs::SelectUIFadeIn()
{
	selectSystem_->SelectUIFadeIn();
}

void GameSceneUIs::ChangeFadeState(BossFadeState changeState)
{
	bossFadeState_ = changeState;
	bossFadeTime_ = 0.0f;
}
