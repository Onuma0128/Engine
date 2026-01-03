#include "PlayerMenuUI.h"

void PlayerMenuUI::Init()
{
	BaseUI::Init("PlayerMenuUI", "GameData");
}

void PlayerMenuUI::Update()
{
	BaseUI::DrawImGui();
	BaseUI::Update();
}

void PlayerMenuUI::Draw()
{
	BaseUI::Draw();
}
