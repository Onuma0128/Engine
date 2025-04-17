#include "Player.h"

void Player::Init()
{
	Object3d::Initialize("Box.obj");

	transform_.scale_ = { 0.3f,0.3f,0.3f };
	transform_.translation_ = { 0.0f,0.3f,0.0f };
}

void Player::GlobalInit()
{

}

void Player::Update()
{
	Object3d::Update();
}

void Player::Draw()
{
	Object3d::Draw();
}
