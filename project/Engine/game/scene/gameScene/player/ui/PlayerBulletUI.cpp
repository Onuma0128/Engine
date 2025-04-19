#include "PlayerBulletUI.h"

void PlayerBulletUI::Init(const Vector2& position)
{
	Sprite::Initialize("uvChecker.png");
	
	size_ = { 32.0f,32.0f };
	position_ = position;
}

void PlayerBulletUI::Update(const Vector2& position)
{
	//transform_.position = position;

	Sprite::Update();
}

void PlayerBulletUI::Draw()
{
	Sprite::Draw();
}
