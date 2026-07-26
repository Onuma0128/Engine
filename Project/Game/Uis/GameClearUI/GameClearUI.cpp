#include "GameClearUI.h"

void GameClearUI::Init()
{
    NumaEngine::BaseUI::Init("GameClearUI", "GameData");
}

void GameClearUI::Update()
{
    NumaEngine::BaseUI::DrawImGui();

	NumaEngine::BaseUI::Update();
}

void GameClearUI::Draw()
{
    NumaEngine::BaseUI::Draw();
}
