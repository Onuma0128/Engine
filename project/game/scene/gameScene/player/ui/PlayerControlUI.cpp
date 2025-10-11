#include "PlayerControlUI.h"

#include "Input.h"

void PlayerControlUI::Init()
{
	BaseUI::Init("PlayerControlUI", "GameData");
}

void PlayerControlUI::Update()
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

void PlayerControlUI::Draw()
{
	BaseUI::Draw();
}
