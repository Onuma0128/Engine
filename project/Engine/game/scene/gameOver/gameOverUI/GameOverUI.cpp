#include "GameOverUI.h"

void GameOverUI::Init()
{
	BaseUI::Init("GameOverUI");
}

void GameOverUI::Update()
{
	BaseUI::DrawImGui();

	BaseUI::Update();
}

void GameOverUI::Draw()
{
	BaseUI::Draw();
}
