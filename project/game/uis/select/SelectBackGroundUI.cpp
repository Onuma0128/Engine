#include "SelectBackGroundUI.h"

void SelectBackGroundUI::Init(const std::string& name)
{
	BaseUI::Init(name, "GameData");
}

void SelectBackGroundUI::Update()
{
	BaseUI::DrawImGui();

	BaseUI::Update();
}

void SelectBackGroundUI::Draw()
{
	BaseUI::Draw();
}
