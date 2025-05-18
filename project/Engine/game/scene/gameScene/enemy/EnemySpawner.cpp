#include "EnemySpawner.h"

void EnemySpawner::Init()
{
	Object3d::Initialize("Box.obj");

	Collider::AddCollider();
	Collider::myType_ = ColliderType::OBB;
	Collider::size_ = transform_.scale_;
}

void EnemySpawner::Update()
{
	for (auto& enemy : enemys_) {
		enemy->Update();
	}

	Collider::centerPosition_ = transform_.translation_;
	Collider::rotate_ = transform_.rotation_;

	Object3d::Update();
}

void EnemySpawner::EnemySpawn()
{
	// 敵を生成
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->Init();
	enemy->GetTransform().translation_ = transform_.translation_;
	enemys_.push_back(std::move(enemy));
}
