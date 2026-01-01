#include "GameSceneUIs.h"

#include "uis/gameSceneUIs/PlayerControlUI.h"
#include "uis/gameSceneUIs/PlayerMenuUI.h"

void GameSceneUIs::Init()
{
	// 調整項目の初期化
	items_ = std::make_unique<GameUiAdjustItem>();
	items_->LoadItems();

	// 操作UIの初期化
	std::unique_ptr<PlayerControlUI> controlUI = std::make_unique<PlayerControlUI>();
	controlUI->Init();
	controlUIs_.push_back(std::move(controlUI));
	std::unique_ptr<PlayerMenuUI> menuUI = std::make_unique<PlayerMenuUI>();
	menuUI->Init();
	controlUIs_.push_back(std::move(menuUI));

	// シーンフェードを初期化
	sceneFade_ = std::make_unique<BaseUI>();
	sceneFade_->Init("GameFade", "GameData", true);
	sceneFade_->GetSprite()->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
	sceneFade_->FadeOut();

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

	for (auto& ui : controlUIs_) {
		ui->Update();
	}

	selectSystem_->Update();
	
	sceneFade_->DrawImGui();
	sceneFade_->Update();

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

void GameSceneUIs::Draw()
{
	for (auto& ui : controlUIs_) {
		ui->Draw();
	}

	killCountUI_->Draw();
	maxKillCountUI_->Draw();
	catUI_->Draw();

	selectSystem_->Draw();

	sceneFade_->Draw();
}

void GameSceneUIs::SelectUIFadeIn()
{
	selectSystem_->SelectUIFadeIn();
}
