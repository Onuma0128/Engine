#include "EnemySpawner.h"

#include "DeltaTimer.h"

#include "gameScene/player/Player.h"
#include "gameScene/gameCamera/GameCamera.h"

void EnemySpawner::Init(SceneObject object)
{
	Object3d::Initialize(object.fileName);
	Object3d::SetSceneRenderer();
	transform_ = object.transform;

	if (object.collider.active) {
		Collider::AddCollider();
		Collider::myType_ = object.collider.type;
		Collider::colliderName_ = "EnemySpawner";
		Collider::offsetPosition_ = object.collider.center;
		Collider::size_ = object.collider.size;
		Collider::DrawCollider();
	}
}

void EnemySpawner::Update()
{
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
	enemy->SetItem(items_);
	enemy->Init();
	enemy->GetTransform().translation_ = transform_.translation_;
	enemys_.push_back(std::move(enemy));
}
