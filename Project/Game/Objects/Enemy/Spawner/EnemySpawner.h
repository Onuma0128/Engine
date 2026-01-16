#pragma once
#include <list>
#include <memory>
#include "objects/enemy/base/BaseEnemy.h"

#include "Object3d.h"
#include "Collider.h"
#include "SceneJsonLoader.h"

/// <summary>
/// 敵発生のスポナークラス
/// </summary>
class EnemySpawner : public Object3d,Collider
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="object"></Jsonの保存データを取得する>
	void Init(SceneObject object);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵をスポーンさせる
	/// </summary>
	/// <param name="enemy"></スポーンさせる敵を入れる>
	void EnemySpawn(BaseEnemy* enemy);

	/// <summary>
	/// スポナーのデータをリセットする
	/// </summary>
	void Reset();

	/* ============================== アクセッサ ============================== */

	/// <summary>
	/// このスポナーが持っている敵のリストを返す
	/// </summary>
	/// <returns></returns>
	std::list<BaseEnemy*>& GetEnemyList() { return enemys_; }

	/// <summary>
	/// このスポナーから出て倒した敵の数を返す
	/// </summary>
	/// <returns></returns>
	uint32_t GetNockdownCount()const { return kNockdownCount_; }

private:

	// 敵のリスト
	std::list<BaseEnemy*> enemys_;
	// 敵を倒した数を保存する
	uint32_t kNockdownCount_ = 0;

	float spawnFrame_ = 0.0f;

};

