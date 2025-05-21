#include "EnemySpawnerFactory.h"

#include "gameScene/player/Player.h"
#include "gameScene/gameCamera/GameCamera.h"

void EnemySpawnerFactory::Init()
{
	CreateSpawner(Vector3{ -4.0f,0.5f,0.0f });
	CreateSpawner(Vector3{ 0.0f,0.5f,0.0f });
	CreateSpawner(Vector3{ 4.0f,0.5f,0.0f });
}

void EnemySpawnerFactory::Update()
{
	for (auto& spawner : enemySpawners_) {
		spawner->Update();
	}
}

void EnemySpawnerFactory::CreateSpawner(const Vector3& position)
{
	std::unique_ptr<EnemySpawner> spawner = std::make_unique<EnemySpawner>();
	spawner->SetPlayer(player_);
	spawner->SetGameCamera(gameCamera_);
	spawner->Init();
	spawner->GetTransform().translation_ = position;
	spawner->EnemySpawn();
	enemySpawners_.push_back(std::move(spawner));
}
