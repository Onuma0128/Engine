#pragma once
#include <list>
#include <memory>
#include "objects/enemy/base/BaseEnemy.h"

#include "Object3d.h"
#include "Collider.h"
#include "SceneJsonLoader.h"

class EnemySpawner : public Object3d,Collider
{
public:

	void Init(SceneObject object);

	void Update();

	void Draw();

	void EnemySpawn(BaseEnemy* enemy);

	/* ============================== アクセッサ ============================== */

	std::list<BaseEnemy*>& GetEnemyList() { return enemys_; }

	uint32_t GetNockdownCount()const { return kNockdownCount_; }

private:

	// 敵のリスト
	std::list<BaseEnemy*> enemys_;
	// 敵を倒した数を保存する
	uint32_t kNockdownCount_ = 0;

	float spawnFrame_ = 0.0f;

};

