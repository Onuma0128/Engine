#pragma once
#include <list>
#include <memory>
#include "gameScene/enemy/Enemy.h"

#include "Object3d.h"
#include "Collider.h"
#include "SceneJsonLoader.h"

class EnemySpawner : public Object3d,Collider
{
public:

	void Init(SceneObject object);

	void Update();

	void Draw();
	
	void EnemySpawn();

	/* ============================== ポインタ ============================== */
	
	Player* GetPlayer() { return player_; }
	void SetPlayer(Player* player) { player_ = player; }

	GameCamera* GetGameCamera() { return gameCamera_; }
	void SetGameCamera(GameCamera* camera) { gameCamera_ = camera; }

	EnemyAdjustItem* GetItem() { return items_; }
	void SetItem(EnemyAdjustItem* item) { items_ = item; }

	/* ============================== アクセッサ ============================== */

	std::list<std::unique_ptr<Enemy>>& GetEnemyList() { return enemys_; }

private:

	// ゲッターで貰ったポインタ
	Player* player_ = nullptr;
	GameCamera* gameCamera_ = nullptr;
	EnemyAdjustItem* items_ = nullptr;

	// 敵のリスト
	std::list<std::unique_ptr<Enemy>> enemys_;

	float spawnFrame_ = 0.0f;

};

