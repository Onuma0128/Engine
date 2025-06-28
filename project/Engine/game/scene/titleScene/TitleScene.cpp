#include "TitleScene.h"

#include "SceneManager.h"
#include "Input.h"

void TitleScene::Initialize()
{
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_A)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}
}

void TitleScene::Draw()
{
}
