#include "TitleUI.h"

void TitleUI::Init()
{
	BaseUI::Init("TitleUI", "GameData", true);
}

void TitleUI::Update()
{
	BaseUI::DrawImGui();

	BaseUI::Update();
}

void TitleUI::Draw()
{
	BaseUI::Draw();
}
