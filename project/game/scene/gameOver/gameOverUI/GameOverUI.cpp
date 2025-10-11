#include "GameOverUI.h"

void GameOverUI::Init()
{
	BaseUI::Init("GameOverUI", "GameData");
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
