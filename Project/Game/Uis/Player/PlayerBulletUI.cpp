#include "PlayerBulletUI.h"

void PlayerBulletUI::Init(const NumaEngine::Vector2& position)
{
	Sprite::Initialize("bulletUI.png");
	
	transform_.position = position;
}

void PlayerBulletUI::Update(const NumaEngine::Vector2& size, const NumaEngine::Vector2& position)
{
	transform_.size = size;
	transform_.position = position;

	Sprite::Update();
}

void PlayerBulletUI::Draw()
{
	Sprite::Draw();
}


