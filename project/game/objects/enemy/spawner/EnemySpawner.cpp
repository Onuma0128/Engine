#include "EnemySpawner.h"

#include "DeltaTimer.h"

void EnemySpawner::Init(SceneObject object)
{
	Object3d::Initialize(object.fileName);
	transform_ = object.transform;
#ifdef _DEBUG
	Object3d::SetSceneRenderer();
#endif // _DEBUG


	if (object.collider.active) {
		Collider::AddCollider();
		Collider::myType_ = object.collider.type;
		Collider::colliderName_ = object.tag;
		Collider::offsetPosition_ = object.collider.center;
		Collider::size_ = object.collider.size;
		Collider::DrawCollider();
	}
}

void EnemySpawner::Update()
{
	for (auto it = enemys_.begin(); it != enemys_.end(); ) {
		(*it)->Update();

		if ((*it)->GetIsDead()) {
			(*it)->Dead();
			++kNockdownCount_;
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

void EnemySpawner::EnemySpawn(BaseEnemy* enemy)
{
	// 敵を生成
	Vector3 pos = transform_.translation_;
	enemy->Reset(pos);
	enemys_.push_back(enemy);
}
