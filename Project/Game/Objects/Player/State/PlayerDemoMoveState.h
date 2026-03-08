#pragma once

#include <vector>
#include <random>

#include "PlayerBaseState.h"

#include "Vector3.h"

/// <summary>
/// プレイヤーのデモ移動
/// </summary>
class PlayerDemoMoveState : public PlayerBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player"></param>
	PlayerDemoMoveState(Player* player);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	PlayerState GetState()const override { return PlayerState::DemoMove; }

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

	// 探索の初期化
	void InitSearch();

	// 探索をリセットする
	void ResetSearch();

private:

	// 乱数生成器の初期化
	std::random_device seedGenerator_;

	// プレイヤーが動くポイント
	std::vector<Vector3> checkPoints_;

	// 今向かっているポイント
	Vector3 nowCheckPoint_;

};

