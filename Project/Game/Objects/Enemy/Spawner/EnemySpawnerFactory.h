#pragma once
#include <memory>
#include <vector>
#include <list>
#include <random>

#include "SceneJsonLoader.h"

#include "EnemySpawner.h"
#include "Objects/Enemy/Base/BaseEnemy.h"
#include "Objects/Enemy/AdjustItem/EnemyAdjustItem.h"
#include "Uis/Player/NumberCountUI.h"

/// <summary>
/// スポナーを管理するクラス
/// </summary>
class EnemySpawnerFactory
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="loader"></Jsonの保存データを取得する>
	void Init(SceneJsonLoader loader);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーを設定する
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// ゲームカメラを設定する
	/// </summary>
	/// <param name="camera"></param>
	void SetGameCamera(GameCamera* camera) { gameCamera_ = camera; }

	/// <summary>
	/// マップデータを設定する
	/// </summary>
	/// <param name="mapData"></param>
	void SetMapData(MapCollision* mapData) { mapData_ = mapData; }

	/// <summary>
	/// 敵を指定座標に湧かせる
	/// </summary>
	/// <param name="type"></湧かせたい敵のタイプ>
	/// <param name="position"></湧かせたい座標>
	void SetSpawnEnemy(EnemyType type, const Vector3& position);

	/// <summary>
	/// 敵のキル数を取得する
	/// </summary>
	/// <returns></returns>
	const uint32_t GetKnockdownCount()const { return knockdownCount_; }

	/// <summary>
	/// スポナーのリセットを行う
	/// </summary>
	void Reset();

private:

	/// <summary>
	/// スポナーを作成する
	/// </summary>
	/// <param name="object"></Jsonの保存データを取得する>
	void CreateSpawner(SceneObject object);

	/// <summary>
	/// 敵のタイプごとの初期化
	/// </summary>
	/// <param name="type"></敵のタイプ>
	/// <param name="enemys"></敵のリスト>
	/// <param name="size"></敵の作成する数>
	void InitTypeEnemy(EnemyType type, std::list<std::unique_ptr<BaseEnemy>>& enemys, size_t size);

	/// <summary>
	/// 敵のタイプごとの更新
	/// </summary>
	/// <param name="enemys"></param>
	void UpdateTypeEnemy(std::list<std::unique_ptr<BaseEnemy>>& enemys);

	/// <summary>
	/// 敵のランダムスポーン関数
	/// </summary>
	void RandomSpawnEnemy();

	/// <summary>
	/// 敵のタイプごとのリセット(デバッグ用)
	/// </summary>
	/// <param name="enemys"></敵のリスト>
	/// <param name="spawner"></スポナーを決める>
	void ResetTypeEnemy(std::list<std::unique_ptr<BaseEnemy>>& enemys, std::unique_ptr<EnemySpawner>& spawner);

	const std::list<std::unique_ptr<BaseEnemy>>& GetEnemys(EnemyType type) {
		switch (type)
		{
		case EnemyType::kMelee:			return enemyMelees_;		break;
		case EnemyType::kRanged:		return enemyRnageds_;		break;
		case EnemyType::kShieldBearer:	return enemyShieldBearers_;	break;
		case EnemyType::kRangedElite:	return enemyRnagedElites_;	break;
		default:										break;
		}
		return enemyMelees_;
	}

private:

	// ゲッターで貰ったポインタ
	Player* player_ = nullptr;
	GameCamera* gameCamera_ = nullptr;
	MapCollision* mapData_ = nullptr;

	// 敵の調整項目
	std::unique_ptr<EnemyAdjustItem> items_ = nullptr;

	// 敵のスポナーリスト
	std::vector<std::unique_ptr<EnemySpawner>> enemySpawners_;

	// 敵のリスト
	std::list<std::unique_ptr<BaseEnemy>> enemyMelees_;			// 近接リスト
	std::list<std::unique_ptr<BaseEnemy>> enemyRnageds_;		// 遠距離リスト
	std::list<std::unique_ptr<BaseEnemy>> enemyShieldBearers_;	// 盾持ちリスト
	std::list<std::unique_ptr<BaseEnemy>> enemyRnagedElites_;	// 遠距離(強化版)リスト
	uint32_t spawnCount_ = 0;

	// 敵のキル数
	uint32_t knockdownCount_ = 0;

	// ランダムデバイス
	std::random_device seedGenerator_;
	// スポーンの間隔とスポーンする時間
	float spawnInterval_ = 0.0f;
	float spawnTime_ = 0.0f;

};