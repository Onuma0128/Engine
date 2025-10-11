#include "PlayerMenuUI.h"

#include "Input.h"

void PlayerMenuUI::Init()
{
	BaseUI::Init("PlayerMenuUI", "GameData");
}

void PlayerMenuUI::Update()
{
	BaseUI::DrawImGui();

	Input* input = Input::GetInstance();

	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_START)) {
		if (!isPlay_) {
			BaseUI::FadeIn();
			isPlay_ = true;
		} else if (isPlay_) {
			BaseUI::FadeOut();
			isPlay_ = false;
		}
	}

	BaseUI::Update();
}

void PlayerMenuUI::Draw()
{
	BaseUI::Draw();
}
