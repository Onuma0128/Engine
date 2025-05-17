#include "EnemySpawner.h"

void EnemySpawner::Init()
{
	Object3d::Initialize("Box.obj");
}

void EnemySpawner::Update()
{
	for (auto& enemy : enemys_) {
		enemy->Update();
	}

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
