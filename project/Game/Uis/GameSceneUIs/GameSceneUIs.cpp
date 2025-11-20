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

	selectSystem_ = std::make_unique<SelectSystem>();
	selectSystem_->Init();

	Update();
}

void GameSceneUIs::Update()
{
	for (auto& ui : controlUIs_) {
		ui->Update();
	}

	selectSystem_->Update();
}

void GameSceneUIs::Draw()
{
	for (auto& ui : controlUIs_) {
		ui->Draw();
	}

	selectSystem_->Draw();
}

void GameSceneUIs::SelectUIFadeIn()
{
	selectSystem_->SelectUIFadeIn();
}
