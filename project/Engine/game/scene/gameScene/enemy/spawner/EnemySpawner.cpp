#include "EnemySpawner.h"

#include "DeltaTimer.h"

#include "gameScene/player/Player.h"
#include "gameScene/gameCamera/GameCamera.h"

void EnemySpawner::Init()
{
	Object3d::Initialize("Box.obj");
	Object3d::SetSceneRenderer();

	Collider::AddCollider();
	Collider::myType_ = ColliderType::OBB;
	Collider::colliderName_ = "EnemySpawner";
	Collider::size_ = transform_.scale_;
	Collider::DrawCollider();
}

void EnemySpawner::Update()
{
	/*spawnFrame_ += DeltaTimer::GetDeltaTime();
	if (spawnFrame_ > 5.0f) {
		spawnFrame_ = 0.0f;
		EnemySpawn();
	}*/
	if (enemys_.size() == 0) {
		EnemySpawn();
	}

	for (auto it = enemys_.begin(); it != enemys_.end(); ) {
		(*it)->Update();

		if ((*it)->GetIsDead()) {
			(*it)->Finalize();
			it = enemys_.erase(it);
		} else {
			++it;
		}
	}

	Collider::rotate_ = transform_.rotation_;
	Collider::centerPosition_ = transform_.translation_;
	Collider::Update();

	Object3d::Update();
}

void EnemySpawner::Draw()
{
	for (auto& enemy : enemys_) {
		enemy->Draw();
	}
}

void EnemySpawner::EnemySpawn()
{
	// 敵を生成
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->SetPlayer(player_);
	enemy->SetGameCamera(gameCamera_);
	enemy->Init();
	enemy->GetTransform().translation_ = transform_.translation_;
	enemys_.push_back(std::move(enemy));
}
