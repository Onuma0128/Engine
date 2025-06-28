#include "TitleUI.h"

void TitleUI::Init()
{
	Sprite::Initialize("title.png");
	Sprite::SetSceneRenderer();
	Sprite::anchorPoint_ = { 0.5f,0.5f };
	Sprite::transform_.position = { 640.0f,360.0f };
	Sprite::transform_.size = { 768.0f,512.0f };
}

void TitleUI::Update()
{
	Sprite::Update();
}
