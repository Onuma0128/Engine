#include "Enemy.h"

void Enemy::Init()
{
	Object3d::Initialize("Box.obj");

	transform_.scale_ = { 0.5f,0.5f,0.5f };
	transform_.translation_ = { 0.0f,0.5f,0.0f };

	effect_ = std::make_unique<EnemyEffect>();
	effect_->SetEnemy(this);
	effect_->Init();
}

void Enemy::Update()
{
	Object3d::Update();
}

void Enemy::Draw()
{
	Object3d::Draw();
}
