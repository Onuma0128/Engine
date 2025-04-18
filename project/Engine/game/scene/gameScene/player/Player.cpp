#include "Player.h"

void Player::Init()
{
	Object3d::Initialize("Box.obj");

	transform_.scale_ = { 0.3f,0.3f,0.3f };
	transform_.translation_ = { 0.0f,0.3f,0.0f };

	effect_ = std::make_unique<PlayerEffect>();
	effect_->Init();
}

void Player::GlobalInit()
{

}

void Player::Update()
{
	Object3d::Update();

	effect_->Update();
}

void Player::Draw()
{
	Object3d::Draw();

	effect_->Draw();
}
