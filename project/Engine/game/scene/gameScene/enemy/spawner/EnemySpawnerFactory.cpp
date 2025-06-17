#include "EnemySpawnerFactory.h"

#include "gameScene/player/Player.h"
#include "gameScene/gameCamera/GameCamera.h"

void EnemySpawnerFactory::Init(SceneJsonLoader loader)
{
	items_ = std::make_unique<EnemyAdjustItem>();
	items_->LoadItems();
	
	for (auto it = loader.GetData().begin(); it != loader.GetData().end();) {
		if (it->second.tag == "EnemySpawner") {
			CreateSpawner(it->second);
		}
		++it;
	}
}

void EnemySpawnerFactory::Update()
{
	items_->Editor();

	for (auto& spawner : enemySpawners_) {
		spawner->Update();
	}
}

void EnemySpawnerFactory::Draw()
{
	for (auto& spawner : enemySpawners_) {
		spawner->Draw();
	}
}

void EnemySpawnerFactory::CreateSpawner(SceneObject object)
{
	std::unique_ptr<EnemySpawner> spawner = std::make_unique<EnemySpawner>();
	spawner->SetPlayer(player_);
	spawner->SetGameCamera(gameCamera_);
	spawner->SetItem(items_.get());
	spawner->Init(object);
	spawner->EnemySpawn();
	enemySpawners_.push_back(std::move(spawner));
}
