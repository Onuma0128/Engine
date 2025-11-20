#include "SelectUI.h"

void SelectUI::Init(const std::string& name)
{
	BaseUI::Init(name, "GameData");
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
