#include "GameClearUI.h"

void GameClearUI::Init()
{
	BaseUI::Init("GameClearUI", "GameData");
}

void GameClearUI::Update()
{
	BaseUI::DrawImGui();

	BaseUI::Update();
}

void GameClearUI::Draw()
{
	BaseUI::Draw();
}
