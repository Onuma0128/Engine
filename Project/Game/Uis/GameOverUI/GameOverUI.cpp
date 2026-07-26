#include "GameOverUI.h"

void GameOverUI::Init()
{
    NumaEngine::BaseUI::Init("GameOverUI", "GameData");
}

void GameOverUI::Update()
{
    NumaEngine::BaseUI::DrawImGui();

	NumaEngine::BaseUI::Update();
}

void GameOverUI::Draw()
{
    NumaEngine::BaseUI::Draw();
}
