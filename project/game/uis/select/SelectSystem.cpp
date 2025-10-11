#include "SelectSystem.h"

#include "Input.h"

void SelectSystem::Init()
{
	backGroundUIs_[0] = std::make_unique<SelectBackGroundUI>();
	backGroundUIs_[0]->Init("SelectBackWidth");
	backGroundUIs_[1] = std::make_unique<SelectBackGroundUI>();
	backGroundUIs_[1]->Init("SelectBackHeight");

	killCountUI_ = std::make_unique<PlayerKillCountUI>();
	killCountUI_->Init();
	killCountUI_->SetPosition(Vector2{ 640.0f,360.0f });
}

void SelectSystem::Update()
{
	for (auto& back : backGroundUIs_) {
		back->Update();
	}

	killCountUI_->Update(0);
}

void SelectSystem::Draw()
{
	for (auto& back : backGroundUIs_) {
		back->Draw();
	}

	//killCountUI_->Draw();
}