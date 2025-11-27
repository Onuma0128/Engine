#include "PlayerBulletUI.h"

void PlayerBulletUI::Init(const Vector2& position)
{
	Sprite::Initialize("bulletUI.png");
	
	transform_.position = position;
}

void PlayerBulletUI::Update(const Vector2& size, const Vector2& position)
{
	transform_.size = size;
	transform_.position = position;

	Sprite::Update();
}

void PlayerBulletUI::Draw()
{
	Sprite::Draw();
}

