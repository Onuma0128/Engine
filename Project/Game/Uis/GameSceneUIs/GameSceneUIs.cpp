#include "GameSceneUIs.h"

#include "uis/gameSceneUIs/PlayerControlUI.h"
#include "uis/gameSceneUIs/PlayerMenuUI.h"

void GameSceneUIs::Init()
{
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

	Update();
}

void GameSceneUIs::Update()
{
	for (auto& ui : controlUIs_) {
		ui->Update();
	}

	selectSystem_->Update();
	
	sceneFade_->DrawImGui();
	sceneFade_->Update();
}

void GameSceneUIs::Draw()
{
	for (auto& ui : controlUIs_) {
		ui->Draw();
	}

	selectSystem_->Draw();

	sceneFade_->Draw();
}

void GameSceneUIs::SelectUIFadeIn()
{
	selectSystem_->SelectUIFadeIn();
}
