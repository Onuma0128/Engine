#include "TitleUI.h"

void TitleUI::Init()
{
    NumaEngine::BaseUI::Init("TitleUI", "GameData", true);
}

void TitleUI::Update()
{
    NumaEngine::BaseUI::DrawImGui();

	NumaEngine::BaseUI::Update();
}

void TitleUI::Draw()
{
    NumaEngine::BaseUI::Draw();
}
