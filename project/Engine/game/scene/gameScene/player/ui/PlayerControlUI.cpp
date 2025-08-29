#include "PlayerControlUI.h"

void PlayerControlUI::Init()
{
	BaseUI::Init("PlayerControlUI");
}

void PlayerControlUI::Update()
{
	BaseUI::DrawImGui();

	BaseUI::Update();
}

void PlayerControlUI::Draw()
{
	BaseUI::Draw();
}
