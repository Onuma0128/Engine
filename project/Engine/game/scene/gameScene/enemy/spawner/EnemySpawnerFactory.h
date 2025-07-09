#pragma once
#include <memory>
#include <vector>
#include <list>
#include <random>

#include "gameScene/enemy/base/BaseEnemy.h"
#include "EnemySpawner.h"
#include "../adjustItem/EnemyAdjustItem.h"
#include "SceneJsonLoader.h"

class EnemySpawnerFactory
{
public:

	void Init(SceneJsonLoader loader);

	void Update();

	void Draw();

	Player* GetPlayer() { return player_; }
	void SetPlayer(Player* player) { player_ = player; }

	GameCamera* GetGameCamera() { return gameCamera_; }
	void SetGameCamera(GameCamera* camera) { gameCamera_ = camera; }

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