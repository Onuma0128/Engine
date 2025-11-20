#pragma once

#include "PlayerBaseState.h"

/// <summary>
/// プレイヤーの必殺技状態
/// </summary>
class PlayerSpecialMoveState : public PlayerBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player"></param>
	PlayerSpecialMoveState(Player* player);

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

