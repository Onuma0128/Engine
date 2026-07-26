#include "PlayerMenuUI.h"

void PlayerMenuUI::Init()
{
    NumaEngine::BaseUI::Init("PlayerMenuUI", "GameData");
}

void PlayerMenuUI::Update()
{
    NumaEngine::BaseUI::DrawImGui();
	NumaEngine::BaseUI::Update();
}

void PlayerMenuUI::Draw()
{
    NumaEngine::BaseUI::Draw();
}
