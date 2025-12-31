#pragma once

#include "BossBaseState.h"

/// <summary>
/// 敵発生ステートクラス
/// </summary>
class BossEnemySpawnState : public BossBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="boss"></param>
	BossEnemySpawnState(BossEnemy* boss);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	BossState GetState() const override { return BossState::EnemySpawn; }

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

private:

};

