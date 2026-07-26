#include "SelectUI.h"

void SelectUI::Init(const std::string& name)
{
    NumaEngine::BaseUI::Init(name, "GameData", true);
}

void SelectUI::Update()
{
    NumaEngine::BaseUI::DrawImGui();

	NumaEngine::BaseUI::Update();
}

void SelectUI::Draw()
{
    NumaEngine::BaseUI::Draw();
}
