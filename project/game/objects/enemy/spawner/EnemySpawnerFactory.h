#pragma once
#include <memory>
#include <vector>
#include <list>
#include <random>

#include "SceneJsonLoader.h"

#include "EnemySpawner.h"
#include "objects/enemy/base/BaseEnemy.h"
#include "objects/enemy/adjustItem/EnemyAdjustItem.h"

class EnemySpawnerFactory
{
public:

	void Init(SceneJsonLoader loader);

	void Update();

	void Draw();

	void SetPlayer(Player* player) { player_ = player; }

	void SetGameCamera(GameCamera* camera) { gameCamera_ = camera; }

	void SetMapData(MapCollision* mapData) { mapData_ = mapData; }

private:

	// スポナーを作る
	void CreateSpawner(SceneObject object);

	// 敵のタイプごとの初期化
	void InitTypeEnemy(EnemyType type, std::list<std::unique_ptr<BaseEnemy>>& enemys, size_t size);
	// 敵のタイプごとの更新
	void UpdateTypeEnemy(std::list<std::unique_ptr<BaseEnemy>>& enemys);
	// 敵のランダムスポーン関数
	void RandomSpawnEnemy();
	void ResetTypeEnemy(std::list<std::unique_ptr<BaseEnemy>>& enemys, std::unique_ptr<EnemySpawner>& spawner);

private:

	// ゲッターで貰ったポインタ
	Player* player_ = nullptr;
	GameCamera* gameCamera_ = nullptr;
	MapCollision* mapData_ = nullptr;

	// 敵のスポナーリスト
	std::vector<std::unique_ptr<EnemySpawner>> enemySpawners_;

	// 敵のリスト
	std::list<std::unique_ptr<BaseEnemy>> enemyMelees_;			// 近接リスト
	std::list<std::unique_ptr<BaseEnemy>> enemyRnageds_;		// 遠距離リスト
	std::list<std::unique_ptr<BaseEnemy>> enemyShieldBearers_;	// 盾持ちリスト
	std::list<std::unique_ptr<BaseEnemy>> enemyRnagedElites_;	// 遠距離(強化版)リスト

	// ランダムデバイス
	std::random_device seedGenerator_;
	// スポーンの間隔とスポーンする時間
	float spawnInterval_ = 0.0f;
	float spawnTime_ = 0.0f;

	// 敵の調整項目
	std::unique_ptr<EnemyAdjustItem> items_ = nullptr;

};