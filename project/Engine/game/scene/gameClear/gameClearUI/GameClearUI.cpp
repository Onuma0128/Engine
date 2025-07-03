#include "GameClearUI.h"

void GameClearUI::Init()
{
	Sprite::Initialize("gameClear.png");
	Sprite::SetSceneRenderer();
	Sprite::GetRenderOptions().offscreen = false;
	Sprite::anchorPoint_ = { 0.5f,0.5f };
	Sprite::transform_.position = { 640.0f,500.0f };
	Sprite::transform_.size = { 800.0f,120.0f };
}

void GameClearUI::Update()
{
	Sprite::Update();
}
