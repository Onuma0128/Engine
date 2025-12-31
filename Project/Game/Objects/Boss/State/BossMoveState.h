#pragma once

#include "BossBaseState.h"

/// <summary>
/// 移動ステートクラス
/// </summary>
class BossMoveState : public BossBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="boss"></param>
	BossMoveState(BossEnemy* boss);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	BossState GetState() const override { return BossState::Move; }

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
	 
	float searchTimer_ = 0.0f;

	float spawnTimer_ = 0.0f;

};
