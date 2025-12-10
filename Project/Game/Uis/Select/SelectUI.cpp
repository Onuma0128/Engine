#include "SelectUI.h"

void SelectUI::Init(const std::string& name)
{
	BaseUI::Init(name, "GameData", true);
}

void SelectUI::Update()
{
	BaseUI::DrawImGui();

	BaseUI::Update();
}

void SelectUI::Draw()
{
	BaseUI::Draw();
}
